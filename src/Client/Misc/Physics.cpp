#include "Physics.hpp"

Physics::Physics(Tilemap *tm) : _tilemap(tm), _phys(PHYS_NONE)
{}

Physics::~Physics()
{}

bool Physics::IsActive()
{ 
    if (_phys == PHYS_NONE)
        return false;
    return true;
}

void Physics::Update(float x, float y)
{
    _x = x;
    _y = y;
    switch (_phys)
    {
        case PHYS_NONE:
            _destX = _x;
            _destY = _y;
            break;
        case PHYS_JUMP_INIT:
            _JumpInit();
            break;
        case PHYS_JUMP_POSY:
            _DoJumpPositiveY();
            break;
        case PHYS_JUMP_NEGY:
            _DoJumpNegativeY();
            break;
        case PHYS_FORCE_GROUND:
            break;
    }
}

void Physics::Jump(float destX, float destY)
{
    _stepX = destX;
    _stepY = destY;
    _phys = PHYS_JUMP_INIT;
}

void Physics::_JumpInit()
{
    _phys = PHYS_JUMP_NEGY;
    _destY = _y + _stepY;
    _destX = _x + _stepX;    
}

void Physics::_DoJumpPositiveY()
{
    if (_y < _destY)
        _y += 0.1f;
    else if (_y >= _destY)
        _phys = PHYS_NONE;
}

void Physics::_DoJumpNegativeY()
{
    if (_y > _destY)
        _y -= 0.1f;
    else if (_y <= _destY)
    {
        _destY = _y - _stepY;
        _phys = PHYS_JUMP_POSY;
    }
}
