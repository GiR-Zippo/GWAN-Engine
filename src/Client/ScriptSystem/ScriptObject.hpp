/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __SCRIPTOBJECT_HPP__
#define __SCRIPTOBJECT_HPP__

#include "GlobalVars.hpp"
#include "Common.h"
#include "angelscript.h"
#include "wrapper.h"

class CScriptMgr;
class WorldObject;
class Object;
class Shader;
#define twod_collision
#define MAX_OBJECTS 64000
#define MAX_TEXURES 64000
#define MAX_SHADERS 64000

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
        void OnResize(int w, int h);

        ///-Object, Texture, Interactionlists
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

        /**********************************************************************\
        |*                         Object Manipulation                        *|
        |*                     ScriptObjectManipulation.cpp                   *|
        \**********************************************************************/
        float GetResolutionX() { return sGlobalVars->GetCurrentWidth(); }
        float GetResolutionY() { return sGlobalVars->GetCurrentHeight(); }
        /// The the color of an object (ID, r,g,b,a)
        void SetObjectRGBA(uint32 id, float r, float g, float b, float a);
        /// Move an object with ID (x,y)
        void MoveObjectXY(uint32 id, float x, float y);
        /// Move an object with ID (x,y,z)
        void MoveObjectXYZ(uint32 id, float x, float y, float z);
        /// Rotates an object with ID (x,y,z)
        void SetRotationXYZ(uint32 id, float x, float y, float z);
        /// Rotates an object with ID at x-axis
        void RotateObjectX(uint32 id,float x);
        /// Rotates an object with ID at y-axis
        void RotateObjectY(uint32 id,float y);
        /// Rotates an object with ID at z-axis
        void RotateObjectZ(uint32 id,float z);
        /// Resizes a object at x-axis
        void ObjectSizeX(uint32 id, float x);
        /// Resizes a object at y-axis
        void ObjectSizeY(uint32 id, float y);
        
        /// Gets the X-position of an object
        float GetPositionX(uint32 id);
        /// Gets the Y-position of an object
        float GetPositionY(uint32 id);

        /**********************************************************************\
        |*                              Shaderobject                          *|
        |*                          ScriptObjectShader.cpp                    *|
        \**********************************************************************/
        int CreateShader(std::string filename);
        void SetObjectShader(uint16 id, uint16 shader);
        void SetUniform1f(uint16 id, std::string name, float valA);
        void SetUniform2f(uint16 id, std::string name, float valA, float valB);
        void DeleteShader(uint16 id);
        void FlushShaders();

        /**********************************************************************\
        |*                              Spriteobject                          *|
        |*                          ScriptObjectSprite.cpp                    *|
        \**********************************************************************/
        int CreateSprite(float x, float y, float z, std::string tileimage, uint32 tileX, uint32 tileY);
        void SetSpriteAminamtionFrame(uint32 id, uint32 frameid);

        /**********************************************************************\
        |*                              Soundobjects                          *|
        |*                          ScriptObjectSound.cpp                     *|
        \**********************************************************************/
        /// Load a sound (filename)
        /// Returns SoundID
        uint32 LoadSound(std::string file);
        /// Play sound with ID
        void PlaySound(uint32 id);
        /// Stop playing sound with ID
        void StopSound(uint32 id);
        /// Set the Soundvolume (SoundID, volume)
        void SetSoundVolume(uint32 id, float vol);
        /// Get the Sound by filename
        uint32 GetSoundIdByName(std::string name);
        /// Remove the sound (ID)
        void UnloadSound(uint32 id);

        /**********************************************************************\
        |*                              Textobject                            *|
        |*                          ScriptObjectText.cpp                      *|
        \**********************************************************************/
        int CreateTextObject(float x, float y, float z, bool depth, float height, std::string text, std::string font);
        void SetNewText(uint32 id, std::string text);
        void SetTextCenterX(uint32 id, bool arg);
        void SetTextCenterY(uint32 id, bool arg);
        void SetTextboxBackgroundTexture(uint32 id, std::string backgroundTexture);
        int CreateTextbox(float x, float y, float z, bool depth, float height, std::string text, std::string font);

        /**********************************************************************\
        |*                            Texturehandling                         *|
        |*                        ScriptObjectTexture.cpp                     *|
        \**********************************************************************/
        /// Load a Midmap (filename)
        int Load2DMidmap(std::string &str);
        /// Delete texture (textureID)
        void DeleteTexture(uint16 texture);
        /// Flush all Textures
        void FlushTextures();

        /**********************************************************************\
        |*                               Tilemap                              *|
        |*                        ScriptObjectTilemap.cpp                     *|
        \**********************************************************************/
        int CreateTileMap(float x, float y, float z, std::string filename);
        bool TileMapIsCollision(uint32 id, float x, float dx, float y, float dy);
        void SetActiveMap(uint16 id) { _ActiveMap = id; }
        uint16 GetActiveMap() { return _ActiveMap; }

        /**********************************************************************\
        |*                             Worldobjects                           *|
        |*                      ScriptObjectWorldObject.cpp                   *|
        \**********************************************************************/
        int CreateWorldObject(float x, float y, float z, std::string filename);
        int CreateWorldObjectCreature(float x, float y, float z, std::string filename);
        int CreateWorldObjectPlayer(float x, float y, float z, std::string tilesetname);

        ///- Objects
        int CreateObject(float x, float y, float z, float sx, float sy, uint16 tex, bool depth, bool ortho, bool clickable);
        int CreateLoadingObject(float x, float y, float z, float sx, float sy, uint16 texture, bool depth);

        ///- 3DModelle
        int CreateMS3DModel(float x, float y, float z, std::string path, std::string filename);
        int CreateWavefrontModel(float x, float y, float z, std::string path, std::string filename);

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
        Shader *_shader[MAX_SHADERS];
        uint16 _objectIndex;
        uint16 _textureIndex;
        std::string _scriptName;
        
        uint16 _ActiveMap; //Die aktive Tilemap
};
#endif
