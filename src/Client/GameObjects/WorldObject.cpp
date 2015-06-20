/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "WorldObject.hpp"
#include "WorldObjectScript.hpp"
#include "ObjectMgr.hpp"
#include "TextureLoader.hpp"
#include "Player.hpp"

#include "TextObject.hpp"
#include "Tilemap.hpp"
#include "ScriptObject.hpp"

#include "Sprite.hpp"

#include "Physics.hpp"

#include <math.h>


#define STEP_SIZE 0.5f
WorldObject::WorldObject(uint32 id, float x, float y, float z, std::string scriptname, ScriptObject* caller, uint8 type):
    Object(id, x, y, z, 0, 0, 0, false, false, false),
    _worldObjectType(type), _callScript(caller), _movementSpeed(0.1f), _objectIndex(0), _collideWithMap(false), _collideWithObjects(false),
    _scriptName(scriptname), _script(NULL), _map(NULL)
{
    _meType        = GFX_NONE;
    _me            = NULL;
    _initalized    = false;
    sObjectMgr->AddObject(this);
    _moveTX = _x;
    _moveTY = _y;
    _physics = NULL;
}

WorldObject::~WorldObject()
{
    sObjectMgr->RemoveObject(this);
    _callScript->RemoveInteractiveWorldObject(this);

    for (std::map<uint8, Object*>::iterator itr = _objectList.begin(); itr != _objectList.end(); itr++)
        if ((*itr).second)
            delete (*itr).second;

    _objectList.clear();
}

void WorldObject::Update(uint32 diff)
{
    if (_script)
        _scriptMgr->CallUpdate(_script, diff);

    if (!_objectWaypoints.empty())
        UpdateWaypoints();
}


void WorldObject::PrintString(std::string& str)
{
    cout << str << "\n";
}

void WorldObject::PrintUint64(uint64 in)
{
    cout << in << "\n";
}

void WorldObject::Draw()
{
    if (!_initalized)
        return;
        
    if (_me)
    {
        if (_physics && _physics->IsActive())
        {
            std::pair<float, float> pos = _physics->GetActualPosition();
            _me->SetPositionX(pos.first);
            _me->SetPositionY(pos.second);
        }

        switch (_meType)
        {
            case GFX_NONE:
                break;

            case GFX_SPRITE:
                _me->ToSprite()->Draw();
                break;
        }
    }
    
    for (std::map<uint8, Object*>::const_iterator itr = _objectList.begin(); itr != _objectList.end(); itr++)
        if ((*itr).second)
            (*itr).second->Draw();
}

void WorldObject::PostDraw()
{
    if (!_initalized)
    {
        cout << _id << ":" << _scriptName << "\n";

        if (!_scriptName.empty())
        {
            _scriptMgr = new WorldObjectScript();
            int r = _scriptMgr->Init();
            _script = _scriptMgr->CreateController(GetFilePath(_scriptName), this);

            if (_script)
                _scriptMgr->CallOnInit(_script);
        }

        //Lass mal Map holen wenns eine gibt
        if (_callScript)
        {
            uint16 map = _callScript->GetActiveMap();
            if (map != 0)
            {
                _map = _callScript->GetObjectByID(map)->ToTilemap();
                _physics = new Physics(_map);
            }
        }
        _initalized = true;
    }

    ///- Wir haben ein Anzeigeobjekt, naja oder auch nicht
    if (_me)
    {
        switch (_meType)
        {
            case GFX_NONE:
                break;

            case GFX_SPRITE:
                _me->ToSprite()->PostDraw();
                break;
        }
        if (_physics)
            _physics->Update(_me->GetPositionX(), _me->GetPositionY());
    }

    _MoveAnimation();
    for (std::map<uint8, Object*>::const_iterator itr = _objectList.begin(); itr != _objectList.end(); itr++)
        if ((*itr).second)
            (*itr).second->PostDraw();
}

float WorldObject::GetPositionX()
{
    if (!_me)
        return 0;
    return _me->GetPositionX();
}

float WorldObject::GetPositionY()
{
    if (!_me)
        return 0;
    return _me->GetPositionY();
}

///-KeyboardInput
void WorldObject::ActivateKeyInput()
{
    _callScript->AddInteractiveWorldObject(this);
}

void WorldObject::DeactivateKeyInput()
{
    _callScript->RemoveInteractiveWorldObject(this);
}

void WorldObject::AsSprite(std::string filename, uint32 x, uint32 y)
{
    _me = new Sprite(1, _x, _y, _z, filename, x, y);
    _meType = GFX_SPRITE;
}

void WorldObject::SetIdleAnimation(bool left, bool right, bool up, bool down)
{
    if (!_me)
        return;

    switch (_meType)
    {
        case GFX_NONE:
            break;

        case GFX_SPRITE:
            _me->ToSprite()->SetIdleAnimation(left, right, up, down);
            break;
    }
    return;
}

bool WorldObject::MoveUp()
{
    if (!_me)
        return false;

    if (_moveTY != _me->GetPositionY() || _moveTX != _me->GetPositionX())
        return false;

    _me->ToSprite()->MoveUp();
    _moveTY -= STEP_SIZE;
    _MoveAnimation();
    return true;
}

bool WorldObject::MoveDown()
{
    if (!_me)
        return false;

    if (_moveTY != _me->GetPositionY() || _moveTX != _me->GetPositionX())
        return false;

    _me->ToSprite()->MoveDown();
    _moveTY += STEP_SIZE;
    _MoveAnimation();
    return true;
}

bool WorldObject::MoveLeft()
{
    if (!_me)
        return false;

    if (_moveTY != _me->GetPositionY() || _moveTX != _me->GetPositionX())
        return false;

    _me->ToSprite()->MoveLeft();
    _moveTX -= STEP_SIZE;
    _MoveAnimation();
    return true;
}

bool WorldObject::MoveRight()
{
    if (!_me)
        return false;

    if (_moveTY != _me->GetPositionY() || _moveTX != _me->GetPositionX())
        return false;

    _me->ToSprite()->MoveRight();
    _moveTX += STEP_SIZE;
    _MoveAnimation();
    return true;
}
#undef STEP_SIZE

bool WorldObject::Jump()
{
    if (!_me || !_physics || _physics->IsActive())
        return false;
    _physics->Jump(0, -2);
    return true;
}

void WorldObject::MoveToPosition(float x, float y, float z)
{
    Position* pos = new Position();
    pos->x = x;
    pos->y = y;
    pos->z = z;
    _objectWaypoints[0] = pos;
    return;
}

bool WorldObject::UpdateWaypoints()
{
    if (!_me)
        return false;

    if ((int(_objectWaypoints[0]->x) == int(_me->ToSprite()->GetPositionX())) 
        && (int(_objectWaypoints[0]->y) == int(_me->ToSprite()->GetPositionY())))
    {
        _objectWaypoints.clear();
        _moveTX = _me->ToSprite()->GetPositionX();
        _moveTY = _me->ToSprite()->GetPositionY();
        if (_script)
            _scriptMgr->CallOnWaypointReached(_script, 0);

        return true;
    }
    else if ((_objectWaypoints[0]->x < 0) && (_objectWaypoints[0]->x != _me->ToSprite()->GetPositionX()))
        MoveRight();
    else if ((_objectWaypoints[0]->x > 0) && (_objectWaypoints[0]->x != _me->ToSprite()->GetPositionX()))
        MoveLeft();
    else if ((_objectWaypoints[0]->y < 0) && (_objectWaypoints[0]->y != _me->ToSprite()->GetPositionY()))
        MoveUp();
    else if ((_objectWaypoints[0]->y > 0) && (_objectWaypoints[0]->y != _me->ToSprite()->GetPositionY()))
        MoveDown();

    return true;
}

int WorldObject::WriteText(float x, float y, float z, bool depth, float height, std::string text, std::string font)
{
    Object* object = new TextObject(_objectIndex, x, y, z, depth, height, text, font);
    return _AddObject(object);
}

void WorldObject::KeyPressed(uint8 key)
{}

void WorldObject::SetUint32(uint16 ID, uint16 Index, uint32 data)
{
    _callScript->SetUint32(ID, Index, data);
}

void WorldObject::SetUint32Ack(uint16 Index, uint32 data)
{
    if (_script)
        _scriptMgr->CallOnSetData(_script, Index, data);
}

void WorldObject::_MoveAnimation()
{
    
    if (_me->GetPositionX() != _moveTX)
    {
        float diffX = _moveTX - _me->GetPositionX();

        if (fabs(diffX) > _movementSpeed)
            diffX = (diffX > 0) ? _movementSpeed : -_movementSpeed;

        bool collision = false;

        if (_map && _collideWithMap)
            collision = _map->IsPropCollosion(_me->GetPositionX(), diffX, -_me->GetPositionY(), 0);

        if (!collision)
            _me->MoveXY(_me->GetPositionX() + diffX, _me->GetPositionY());
        else
            _moveTX = _me->GetPositionX();
    }

    if (_me->GetPositionY() != _moveTY)
    {
        float diffY = _moveTY - _me->GetPositionY();
        if (fabs(diffY) > _movementSpeed)
            diffY = (diffY > 0) ? _movementSpeed : -_movementSpeed;

        bool collision = false;

        if (_map && _collideWithMap)
            collision = _map->IsPropCollosion(_me->GetPositionX(), 0, -_me->GetPositionY(), diffY);

        if (!collision)
            _me->MoveXY(_me->GetPositionX(), _me->GetPositionY() + diffY);
        else
            _moveTY = _me->GetPositionY();
    }
}

///-SubObjekte
int WorldObject::_AddObject(Object* object)
{
    _objectList[_objectIndex] = object;
    int idx = _objectIndex;
    _objectIndex++;
    return idx;
}

void WorldObject::DeleteObject(uint8 id)
{
    for (std::map<uint8, Object*>::iterator itr = _objectList.begin(); itr != _objectList.end(); itr++)
        if ((*itr).first == id)
        {
            delete (*itr).second;
            _objectList.erase(itr);
            return;
        }
}

///- ScriptAPI
asILockableSharedBool* WorldObject::GetWeakRefFlag()
{
    if ( !weakRefFlag )
        weakRefFlag = asCreateLockableSharedBool();

    return weakRefFlag;
}

int WorldObject::AddRef()
{
    return ++refCount;
}

int WorldObject::Release()
{
    if ( --refCount == 0 )
    {
        delete this;
        return 0;
    }

    return refCount;
}

void WorldObject::DestroyAndRelease()
{
    // Since there might be other object's still referencing this one, we
    // cannot just delete it. Here we will release all other references that
    // this object holds, so it doesn't end up holding circular references.
    if ( _script )
    {
        _script->Release();
        _script = 0;
    }

    Release();
}