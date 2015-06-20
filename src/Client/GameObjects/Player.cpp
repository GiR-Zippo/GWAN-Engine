/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Player.hpp"
#include "ScriptObject.hpp"

#include "TextureLoader.hpp"
#include "WorldObjectScript.hpp"

Player::Player(uint32 id, float x, float y, float z, std::string scriptname, ScriptObject *caller):
WorldObject(id, x, y, z, scriptname, caller, WOT_PLAYER)
{
 temp = 0;
}

Player::~Player()
{

}

void Player::KeyPressed(uint8 key)
{
    if (_script)
        _scriptMgr->CallOnKeyPressed(_script, key);
    
    if (key == '1')
        temp++;
    if (key == '2')
        temp--;
 //cout << key << " " << temp << "\n";   
}
