#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

#include "Common.h"

enum PhysicsZeugs
{
    PHYS_NONE,
    PHYS_JUMP_INIT,
    PHYS_JUMP_POSX,
    PHYS_JUMP_NEGX,
    PHYS_JUMP_POSY,
    PHYS_JUMP_NEGY,
    PHYS_FORCE_GROUND,
    PHYS_MAX
};

class Tilemap;
class Physics
{
    public:
        Physics(Tilemap *tm);
        ~Physics();
        void Update(float x, float y);  ///- Wichtig

        ///- Jumpanimation, springe actPos + destPos
        void Jump(float destX, float destY);
        
        ///- Ist eine "Berechnung" am laufen
        bool IsActive();
        
        ///- Aktuell ausgerechnete Position
        std::pair<float, float> GetActualPosition() { return std::pair <float, float> (_x, _y); }
    
    private:
        void _JumpInit();
        void _DoJumpPositiveX();
        void _DoJumpNegativeX();
        void _DoJumpPositiveY();
        void _DoJumpNegativeY();
    private:
        Tilemap *_tilemap;
        PhysicsZeugs _phys;
        float _x;       //current position
        float _y;
        float _destX;
        float _destY;   //destposition
        float _stepX;
        float _stepY;   //original step
        float _velFac;
};
#endif
