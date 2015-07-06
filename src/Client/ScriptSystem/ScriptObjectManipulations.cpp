#include "ScriptObject.hpp"
#include "Object.hpp"

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
 
