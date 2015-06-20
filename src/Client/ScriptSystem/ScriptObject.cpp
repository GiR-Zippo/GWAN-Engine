/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include <math.h>
#include "TextureLoader.hpp" //Der wills so

#include "ScriptObject.hpp"
#include "scriptmgr.h"
#include "Common.h"

#include "3DObject.hpp"
#include "LoadingObject.hpp"
#include "Sprite.hpp"
#include "Object.hpp"
#include "PlanarObject.hpp"

#include "Tilemap.hpp"

#include "Sound.hpp"
#include "SoundMgr.hpp"

#include "WorldObject.hpp"
#include "Creature.hpp"
#include "Player.hpp"

#include "ObjectMgr.hpp"

ScriptObject::ScriptObject(std::string name)
{
    _ActiveMap = 0;
    _scriptName = name;
    // The first reference is already counted when the object is created
    refCount         = 1;
    _script          = 0;
    weakRefFlag      = 0;
    _objectIndex     = 1;

    // Clear the Objectlists
    for (uint32 i = 0; i != MAX_OBJECTS; i++)
        _objectUpdateList[i] = NULL;
    for (uint32 i = 0; i != MAX_OBJECTS; i++)
        _objectList[i] = NULL;
    for (uint32 i = 0; i != MAX_OBJECTS; i++)
        _interactiveObjects[i] = NULL;
}

ScriptObject::~ScriptObject()
{
    if( weakRefFlag )
    {
        // Tell the ones that hold weak references that the object is destroyed
        weakRefFlag->Set(true);
        weakRefFlag->Release();
    }

    if( _script )
        _script->Release();

    //Release the whole Shit
    for (uint32 i =0; i != MAX_OBJECTS; i++)
    {
        _objectUpdateList[i] = NULL;
        if (_objectList[i])
            delete _objectList[i];
    }
    _cameraViews.clear();
}

asILockableSharedBool *ScriptObject::GetWeakRefFlag()
{
    if( !weakRefFlag )
        weakRefFlag = asCreateLockableSharedBool();

    return weakRefFlag;
}

int ScriptObject::AddRef()
{
    return ++refCount;
}

int ScriptObject::Release()
{
    if( --refCount == 0 )
    {
        delete this;
        return 0;
    }
    return refCount;
}

void ScriptObject::DestroyAndRelease()
{
    // Since there might be other object's still referencing this one, we
    // cannot just delete it. Here we will release all other references that
    // this object holds, so it doesn't end up holding circular references.
    if( _script )
    {
        _script->Release();
        _script = 0;
    }
    Release();
}

/************************************************************\
|*  Alles Funktionen die von OpenGL durchgeschliffen werden *|
\************************************************************/
void ScriptObject::InitGL()
{
    _scriptMgr->CallOnInitGL(_script);
}

void ScriptObject::Draw()
{
    _scriptMgr->CallOnDrawGL(_script);
    for (uint32 i =0; i != MAX_OBJECTS; i++)
    {
        if (_objectUpdateList[i] != NULL)
            _objectUpdateList[i]->Draw();
    }
}

void ScriptObject::PostDraw()
{
    _scriptMgr->CallOnDrawGL(_script);
    for (uint32 i =0; i != MAX_OBJECTS; i++)
    {
        if (_objectUpdateList[i] != NULL)
            _objectUpdateList[i]->PostDraw();
    }
}

void ScriptObject::Tick(uint32 diff)
{
    _scriptMgr->CallOnTick(_script);
}

void ScriptObject::Mouse(int button, int state, int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    for (uint32 i =0; i != MAX_OBJECTS; i++)
    {
        if (_objectUpdateList[i] != NULL)
            if(_objectUpdateList[i]->IsMouseHit(x, y))
            {
                //cout << _objectUpdateList[i]->GetId() << "\n";
                _scriptMgr->CallOnMouseClick(_script, posX, posY, posZ, button, state, _objectUpdateList[i]->GetId());
                return;
            }
    }
    _scriptMgr->CallOnMouseClick(_script, posX, posY, posZ, button, state);
}

void ScriptObject::MouseMove(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    _scriptMgr->CallOnMouseMove(_script, posX, posY);
}

void ScriptObject::KeyPressed(uint8 key)
{
    _scriptMgr->CallOnKeyPressed(_script, key);

    for (uint32 i = 0; i != MAX_OBJECTS; i++)
    {
        if (_interactiveObjects[i] == NULL)
            return;
        _interactiveObjects[i]->KeyPressed(key);
    }
}

/************************************************************\
|*           Hier geht die eigentliche Objectlist los       *|
\************************************************************/
void ScriptObject::FlushTextures()
{
    uint32 vTex = 0;
    for (uint32 i = 0; i != MAX_TEXURES; i++)
    {
        if (glIsTexture(i))
        {
            _textures[vTex] = i;
            vTex++;
        }
    }
    glDeleteTextures(vTex, _textures);
}

void ScriptObject::FlushObjects()
{
    for (GLuint i = 0; i != MAX_TEXURES; i++)
    {
        if (glIsList(i))
        {
            glDeleteLists(i, 1);
        }
    }
}

void ScriptObject::DeleteObject(uint16 id)
{
    _objectUpdateList[id] = NULL;
    delete _objectList[id];
}

void ScriptObject::AddObjectToUpdate(uint16 pos, uint16 id)
{
    _objectUpdateList[pos] = _objectList[id];
}

void ScriptObject::FlushUpdateList()
{
    for (uint32 i =0; i != MAX_OBJECTS; i++)
        _objectUpdateList[i] = NULL;
}

void ScriptObject::RemoveObjectFromUpdateByPosition(uint16 pos)
{
    _objectUpdateList[pos] = NULL;
}

void ScriptObject::RemoveObjectFromUpdate(uint16 id)
{
    for (uint16 i = 0; i != MAX_OBJECTS; i++)
    {
        if (_objectUpdateList[i] != NULL)
            if (_objectList[id] == _objectUpdateList[i])
                _objectUpdateList[i] = NULL;
    }
}

void ScriptObject::AddInteractiveWorldObject(WorldObject* object)
{
    _AddInteractiveObject(object);
}

void ScriptObject::RemoveInteractiveWorldObject(WorldObject* object)
{
    _RemoveInteractiveObject(object);
}


/************************************************************\
|*         Ohne Objects wirds etwas schwarz aufm Schirm     *|
\************************************************************/
void ScriptObject::PrintString(string &str)
{
    cout << str;
}

void ScriptObject::PrintUInt(uint64 data)
{
    cout << data << "\n";
}

void ScriptObject::PrintFloat(float data)
{
    cout << data << "\n";
}

///- Objects
int ScriptObject::CreateObject(float x, float y, float z, float sx, float sy, uint16 tex, bool depth, bool ortho, bool clickable)
{
    Object *object = new PlanarObject(_objectIndex, x, y, z, sx, sy, tex, depth, ortho, clickable);
    return _AddObject(object);
}

int ScriptObject::CreateLoadingObject(float x, float y, float z, float sx, float sy, uint16 texture, bool depth)
{
    Object *object = new LoadingObject(_objectIndex, x, y, z, sx, sy, texture, depth);
    return _AddObject(object);
}

int ScriptObject::CreateSprite(float x, float y, float z, std::string tileimage, uint32 tileX, uint32 tileY)
{
    Object *object = new Sprite(_objectIndex, x, y, z, tileimage, tileX, tileY);
    return _AddObject(object);
}

///- 3DModelle
int ScriptObject::CreateMS3DModel(float x, float y, float z, string path, string filename)
{
    Object *object = new DreiDObject(_objectIndex,x,y,z);
    std::string dPath = GetPath() + path;
    object->To3D()->LoadMilkShapeModelData(dPath.c_str(), filename.c_str());
    return _AddObject(object);
}

int ScriptObject::CreateWavefrontModel(float x, float y, float z, std::string path, std::string filename)
{
    Object *object = new DreiDObject(_objectIndex,x,y,z);
    std::string dPath = GetPath() + path;
    object->To3D()->LoadObjModelData(dPath.c_str(), filename.c_str());
    return _AddObject(object);
}


///- TileMapping
int ScriptObject::CreateTileMap(float x, float y, float z, std::string filename)
{
    Object *object = new Tilemap(_objectIndex, x, y, z, true, GetFilePath(filename));
    return _AddObject(object);
}

void ScriptObject::SetObjectRGBA(uint32 id, float r, float g, float b, float a)
{
    if(_objectList[id])
        _objectList[id]->SetObjectRGBA(r,g,b,a);
}

void ScriptObject::MoveObjectXY(uint32 id, float x, float y)
{
    if(_objectList[id])
        _objectList[id]->MoveXY(x,y);
}

void ScriptObject::MoveObjectXYZ(uint32 id, float x, float y, float z)
{
    if(_objectList[id])
        _objectList[id]->MoveXYZ(x,y,z);
}

void ScriptObject::SetRotationXYZ(uint32 id, float x, float y, float z)
{
    if(_objectList[id])
        _objectList[id]->SetRotationXYZ(x,y,z);
}

void ScriptObject::RotateObjectX(uint32 id, float x)
{
    if(_objectList[id])
        _objectList[id]->SetRotX(x);
}

void ScriptObject::RotateObjectY(uint32 id, float y)
{
    if(_objectList[id])
        _objectList[id]->SetRotY(y);
}

void ScriptObject::RotateObjectZ(uint32 id, float z)
{
    if(_objectList[id])
        _objectList[id]->SetRotZ(z);
}

void ScriptObject::ObjectSizeX(uint32 id, float x)
{
    if(_objectList[id])
        _objectList[id]->SetSizeX(x);
}

void ScriptObject::ObjectSizeY(uint32 id, float y)
{
    if(_objectList[id])
        _objectList[id]->SetSizeY(y);
}

float ScriptObject::GetPositionX(uint32 id)
{
    if(_objectList[id])
        return _objectList[id]->GetPositionX();
    return 0;
}

float ScriptObject::GetPositionY(uint32 id)
{
    if(_objectList[id])
        return _objectList[id]->GetPositionY();
    return 0;
}

///- Textures
int ScriptObject::Load2DMidmap(string &str)
{
    int tex = 0;
    TextureLoader *tload = new TextureLoader();
    tex = tload->Load2DMipmap(GetFilePath(str.c_str()));
    if (tex == 0)
        return 0;
    delete tload;
    return tex;
}

void ScriptObject::DeleteTexture(uint16 texture)
{
    GLuint tex = texture;
    glDeleteTextures(1, &tex);
}

///- Others
int ScriptObject::Rand()
{
    srand (time(NULL));
    return rand();
}

int ScriptObject::RandAB(int lo, int hi)
{
    srand (time(NULL));
    return (rand()%(hi-lo))+ lo;
}

void ScriptObject::SetUint32(uint16 id, uint16 Index, uint32 data)
{
    if(_objectList[id])
        _objectList[id]->ToWorldObject()->SetUint32Ack(Index, data);
}

Object* ScriptObject::GetObjectByID(uint16 id)
{
    if(_objectList[id])
        return _objectList[id];
    return NULL;
}

void ScriptObject::SetRealmlist(string &wow, string &realmlist)
{
    ofstream myfile;
    myfile.open (wow.c_str());
    myfile << realmlist << "\n";
    myfile.close();
    return;
}

void ScriptObject::Test()
{
    //Sound *_sound = new Sound;
   // _sound->LoadSound(GetFilePath("test.wav"));
   // _sound->PlaySound(0);

}

int ScriptObject::_AddObject(Object* object)
{
    _objectList[_objectIndex] = object;
    int idx = _objectIndex;
    _objectIndex++;
    return idx;
}

void ScriptObject::_AddInteractiveObject(Object* object)
{
    for (uint32 i = 0; i != MAX_OBJECTS; i++)
    {
        if (_interactiveObjects[i] == NULL)
        {
            _interactiveObjects[i] = object;
            return;
        }
    }
}

void ScriptObject::_RemoveInteractiveObject(Object* object)
{
    for (uint32 i = 0; i != MAX_OBJECTS; i++)
    {
        if (_interactiveObjects[i] == object)
        {
            _interactiveObjects[i] = NULL;
            return;
        }
    }
}
