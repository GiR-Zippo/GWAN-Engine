/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __CREATURE_HPP__
#define __CREATURE_HPP__

#include "Common.h"
#include "WorldObject.hpp"
#include "Object.hpp"

class Creature : public WorldObject
{
    public:
        Creature(uint32 id, float x, float y, float z, std::string scriptname, ScriptObject *caller);
        ~Creature();

        void Update(uint32 diff);
        void Draw();
    private:
        
};
#endif
