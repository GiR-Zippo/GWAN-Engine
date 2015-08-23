#include "ScriptObject.hpp"
#include "Object.hpp"
#include "Tilemap.hpp"

int ScriptObject::CreateTileMap(float x, float y, float z, std::string filename)
{
    Object *object = new Tilemap(_objectIndex, x, y, z, true, GetFilePath(filename));
    return _AddObject(object);
}

bool ScriptObject::TileMapIsCollision(uint32 id, float x, float dx, float y, float dy)
{
    if(_objectList[id])
        return _objectList[id]->ToTilemap()->IsPropCollosion(x, dx, y, dy);
    return false;
}
