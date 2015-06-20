/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __LOADINGOBJECT_HPP__
#define __LOADINGOBJECT_HPP__

#include "Object.hpp"

class LoadingObject : public Object
{
    public:
        LoadingObject(uint16 id, float x, float y, float z, float sx, float sy, uint16 tex, bool depth);
        void Draw();
    private:
        bool  _open;
        uint8 _smax;
        uint32 _counter;
        float rot;

};
#endif
 
