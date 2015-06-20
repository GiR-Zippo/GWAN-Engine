/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "Common.h"
#include "WorldObject.hpp"

class ScriptObject;
class Player : public WorldObject
{
    public:
        Player(uint32 id, float x, float y, float z, std::string scriptname, ScriptObject *caller);
        ~Player();
        
        void KeyPressed(uint8 key);
};
#endif