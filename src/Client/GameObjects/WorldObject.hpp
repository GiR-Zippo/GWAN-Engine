/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __WORLDOBJECT_HPP__
#define __WORLDOBJECT_HPP__

#include "Common.h"
#include "Object.hpp"

#include "angelscript.h"
#include <map>

#define WAYPOINT_CONST 5

enum WorldObjectType
{
    WOT_NORMAL   = 0,
    WOT_CREATURE = 1,
    WOT_PLAYER   = 2,
    EOT_MAP      = 3,
};

enum GFXObjectType
{
    GFX_NONE     = 0,
    GFX_SPRITE   = 1,
    GFX_MAX      = 2
};

struct Position
{
    float x;
    float y;
    float z;
};

class Physics;
class ScriptObject;
class Tilemap;
class WorldObjectScript;
class WorldObject : public Object
{
        friend class Player;
    public:
        WorldObject (uint32 id, float x, float y, float z, std::string scriptname, ScriptObject* caller, uint8 type = WOT_NORMAL );
        ~WorldObject();
        virtual void Update ( uint32 diff );

        void PrintString ( std::string& str );
        void PrintUint64 (uint64 in);

        //Die kommt vom ObjectMgr
        virtual void Draw();
        virtual void PostDraw();
        virtual float GetPositionX();
        virtual float GetPositionY();

        ///-Tastatureingabe
        void ActivateKeyInput();
        void DeactivateKeyInput();

        ///-Texturzeugs
        void AsSprite(std::string filename, uint32 x, uint32 y);

        ///-Kollisionskontrolle
        void SetCollisionWithObjects ( bool in ) { _collideWithObjects = in; }
        void SetCollisionWithMap ( bool in ) { _collideWithMap = in; }

        ///-Animation and Movement
        void SetMovementSpeed ( float speed ) { _movementSpeed = speed; }
        void SetIdleAnimation ( bool left, bool right, bool up, bool down );
        bool MoveUp();
        bool MoveDown();
        bool MoveLeft();
        bool MoveRight();
        bool Jump();
        void MoveToPosition ( float x, float y, float z );
        bool UpdateWaypoints();


        ///- GroundForce
        void GroundForce(bool on);
        
        ///-Subobjekte
        void DeleteObject ( uint8 id );

        ///-Textausgabe ;)
        int WriteText ( float x, float y, float z, bool depth, float height, std::string text, std::string font );

        ///-Interop
        virtual void KeyPressed ( uint8 key );
        void SetUint32 ( uint16 ID, uint16 Index, uint32 data );
        void SetUint32Ack ( uint16 Index, uint32 data );
        uint8 GetType() { return _worldObjectType; }

        //CollisionControl
        bool CanCollideWithMap()
        {
            return _collideWithMap;    //Others can collide
        }
        bool CanCollideWithObjects()
        {
            return _collideWithObjects;    //Can collide with others
        }

    private:
        void _MoveAnimation();
        float _moveTX;
        float _moveTY;
        GFXObjectType                   _meType;
        Object*                         _me;
        Tilemap*                        _map;
        Physics*                        _physics;
        float                           _movementSpeed;
        std::map<uint8, Position*>      _objectWaypoints;
        std::map<uint8, Object*>        _objectList;  //Objekte die nur auf dieses bezug haben
        uint8                           _objectIndex;
        int                             _AddObject ( Object* object );

        uint8  temp;
        uint8                           _worldObjectType;
        bool                            _collideWithMap;
        bool                            _collideWithObjects;
        ///-Script API
    public:
        int AddRef();
        int Release();
        void DestroyAndRelease();
        asILockableSharedBool* GetWeakRefFlag();

    protected:
        int                    refCount;
        asILockableSharedBool* weakRefFlag;

    private:
        std::string                     _scriptName;
        bool                            _initalized;
        ScriptObject*                   _callScript;
        WorldObjectScript*              _scriptMgr;
        asIScriptObject*                _script;
};
#endif


