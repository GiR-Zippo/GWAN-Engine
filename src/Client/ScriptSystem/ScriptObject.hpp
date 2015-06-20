/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __SCRIPTOBJECT_HPP__
#define __SCRIPTOBJECT_HPP__
#include "Common.h"
#include "angelscript.h"
#include "wrapper.h"

class CScriptMgr;
class WorldObject;
class Object;
#define twod_collision
#define MAX_OBJECTS 64000
#define MAX_TEXURES 64000

class ScriptObject : public Wrapper
{
    public:
        ScriptObject(std::string name);
        asILockableSharedBool *GetWeakRefFlag();

        void SetScriptObject(asIScriptObject *script, CScriptMgr *scriptMgr) 
        { 
            _script = script; 
            _scriptMgr = scriptMgr;  
        }

        int AddRef();
        int Release();
        void DestroyAndRelease();

        ///-Funtionen die von OpenGL kommen
        void InitGL();
        void Draw();
        void PostDraw();
        void Tick(uint32 diff);
        void Mouse(int button, int state, int x, int y);
        void MouseMove(int x, int y);
        void KeyPressed(uint8 key);

        ///-Object, Texture, Interactionlists
        void FlushTextures();
        void FlushObjects();
        void DeleteObject(uint16 id);
        void AddObjectToUpdate(uint16 pos, uint16 id);
        void FlushUpdateList();
        void RemoveObjectFromUpdateByPosition(uint16 pos);
        void RemoveObjectFromUpdate(uint16 id);
        void AddInteractiveWorldObject(WorldObject *object);
        void RemoveInteractiveWorldObject(WorldObject *object);

        void PrintString(std::string &str);
        void PrintUInt(uint64 data);
        void PrintFloat(float data);

        ///- Objects
        int CreateObject(float x, float y, float z, float sx, float sy, uint16 tex, bool depth, bool ortho, bool clickable);
        int CreateLoadingObject(float x, float y, float z, float sx, float sy, uint16 texture, bool depth);
        int CreateSprite(float x, float y, float z, std::string tileimage, uint32 tileX, uint32 tileY);
        
        int CreateTextbox(float x, float y, float z, bool depth, float height, std::string text, std::string font);
        void SetTextboxBackgroundTexture(uint32 id, std::string backgroundTexture);

        
        ///- 3DModelle
        int CreateMS3DModel(float x, float y, float z, std::string path, std::string filename);
        int CreateWavefrontModel(float x, float y, float z, std::string path, std::string filename);

        ///- TileMapping
        int CreateTileMap(float x, float y, float z, std::string filename);
        void SetActiveMap(uint16 id) { _ActiveMap = id; }
        uint16 GetActiveMap() { return _ActiveMap; }

        void SetObjectRGBA(uint32 id, float r, float g, float b, float a);
        void MoveObjectXY(uint32 id, float x, float y);
        void MoveObjectXYZ(uint32 id, float x, float y, float z);
        void SetRotationXYZ(uint32 id, float x, float y, float z);
        void RotateObjectX(uint32 id,float x);
        void RotateObjectY(uint32 id,float y);
        void RotateObjectZ(uint32 id,float z);
        void ObjectSizeX(uint32 id, float x);
        void ObjectSizeY(uint32 id, float y);
        
        float GetPositionX(uint32 id);
        float GetPositionY(uint32 id);

        ///- ScriptObjectText.cpp
        int CreateTextObject(float x, float y, float z, bool depth, float height, std::string text, std::string font);
        void SetNewText(uint32 id, std::string text);
        void SetTextCenterX(uint32 id, bool arg);
        void SetTextCenterY(uint32 id, bool arg);

        ///- ScriptWorldObject.cpp
        int CreateWorldObject(float x, float y, float z, std::string filename);
        int CreateWorldObjectCreature(float x, float y, float z, std::string filename);
        int CreateWorldObjectPlayer(float x, float y, float z, std::string tilesetname);

        ///- ScriptObjectSound.cpp
        uint32 LoadSound(std::string file);
        void PlaySound(uint32 id);
        void StopSound(uint32 id);
        void SetSoundVolume(uint32 id, float vol);
        uint32 GetSoundIdByName(std::string name);
        void UnloadSound(uint32 id);
        
        ///- Textures
        int Load2DMidmap(std::string &str);
        void DeleteTexture(uint16 texture);

        ///- Other things
        int Rand();
        int RandAB(int lo, int hi);

        void Test();
        //Interop
        void SetUint32(uint16 id, uint16 Index, uint32 data);
        Object *GetObjectByID(uint16 id);
        //IO
        void SetRealmlist(std::string &wow, std::string &realmlist);

        std::string GetScriptName() const { return _scriptName; }
    protected:
        ~ScriptObject();
        int                    refCount;
        asILockableSharedBool *weakRefFlag;

    private:
        int _AddObject(Object* object);
        void _AddInteractiveObject(Object* object);
        void _RemoveInteractiveObject(Object* object);

    private:
        asIScriptObject *_script;
        CScriptMgr *_scriptMgr;
        Object *_objectList[MAX_OBJECTS];
        Object *_objectUpdateList[MAX_OBJECTS];
        Object *_interactiveObjects[MAX_OBJECTS];
        std::map<float, std::pair<float,float> > _cameraViews;
        GLuint _textures[MAX_TEXURES];
        uint16 _objectIndex;
        uint16 _textureIndex;
        std::string _scriptName;
        
        uint16 _ActiveMap; //Die aktive Tilemap
};
#endif
