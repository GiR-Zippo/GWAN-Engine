/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Creature.hpp"

Creature::Creature(uint32 id, float x, float y, float z, std::string scriptname, ScriptObject *caller):
WorldObject(id, x, y, z, scriptname, caller, WOT_CREATURE)
{}

Creature::~Creature()
{}

void Creature::Update(uint32 diff)
{
    WorldObject::Update(diff);
}

void Creature::Draw()
{
    WorldObject::Draw();
}
