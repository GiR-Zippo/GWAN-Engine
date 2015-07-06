#include "ScriptObject.hpp"
#include "Object.hpp"
#include "Sprite.hpp"

int ScriptObject::CreateSprite(float x, float y, float z, std::string tileimage, uint32 tileX, uint32 tileY)
{
    Object *object = new Sprite(_objectIndex, x, y, z, tileimage, tileX, tileY);
    return _AddObject(object);
}

void ScriptObject::SetSpriteAminamtionFrame(uint32 id, uint32 frameid)
{
    if(_objectList[id])
        _objectList[id]->ToSprite()->SetAminamtionFrame(frameid);
}
