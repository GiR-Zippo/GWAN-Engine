/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/
#include "Common.h"
#include "ScriptObject.hpp"
#include "Textbox.hpp"
#include "TextObject.hpp"
#include "Object.hpp"

int ScriptObject::CreateTextObject(float x, float y, float z, bool depth, float height, std::string text, std::string font)
{
    Object *object = new TextObject(_objectIndex,x,y,z, depth, height, text, font);
    return _AddObject(object);
}

void ScriptObject::SetNewText(uint32 id, std::string text)
{
    if(_objectList[id])
        _objectList[id]->ToText()->SetText(text);
}

void ScriptObject::SetTextCenterX(uint32 id, bool arg)
{
    if(_objectList[id])
        _objectList[id]->ToText()->SetCenterX(arg);
}

void ScriptObject::SetTextCenterY(uint32 id, bool arg)
{
    if(_objectList[id])
        _objectList[id]->ToText()->SetCenterY(arg);
}


///- Textbox
int ScriptObject::CreateTextbox(float x, float y, float z, bool depth, float height, std::string text, std::string font)
{
    Object *object = new Textbox(_objectIndex,x,y,z, depth, height, text, font);
    return _AddObject(object);
} 

void ScriptObject::SetTextboxBackgroundTexture(uint32 id, std::string backgroundTexture)
{
    if(_objectList[id])
        _objectList[id]->ToTextbox()->SetbackgroundTexture(backgroundTexture);
}
