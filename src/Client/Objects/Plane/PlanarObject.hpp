/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __PLANAROBJECT_HPP__
#define __PLANAROBJECT_HPP__

#include "Object.hpp"
class PlanarObject : public Object
{
    public:
        PlanarObject(uint16 id, float x, float y, float z, float sx, float sy, uint16 tex, bool depth, bool ortho, bool clickable);
        void Draw();
    private:

};
#endif
