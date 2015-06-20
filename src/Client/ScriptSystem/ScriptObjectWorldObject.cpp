/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Common.h"
#include "WorldObject.hpp"
#include "ScriptObject.hpp"
#include "Object.hpp"
#include "Creature.hpp"
#include "Player.hpp"

int ScriptObject::CreateWorldObject(float x, float y, float z, std::string filename)
{
    Object *object = new WorldObject(_objectIndex, x,y,z,filename, this);
    return _AddObject(object);
}

int ScriptObject::CreateWorldObjectCreature(float x, float y, float z, std::string filename)
{
    Object *object = new Creature(_objectIndex, x,y,z,filename, this);
    return _AddObject(object);
}

int ScriptObject::CreateWorldObjectPlayer(float x, float y, float z, string tilesetname)
{
    Object *object = new Player(_objectIndex, x, y, z, tilesetname, this);
    return _AddObject(object);
} 
