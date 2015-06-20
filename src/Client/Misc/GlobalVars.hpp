#ifndef __GLOABLVARS_HPP__
#define __GLOABLVARS_HPP__

#include <ace/Singleton.h>
#include <ace/Atomic_Op.h>
#include <ace/Basic_Types.h>
#include <ace/Guard_T.h>
#include <ace/Null_Mutex.h>
#include <ace/Thread_Mutex.h>

class GlobalVars
{
    public:
        GlobalVars() {}
        void    SetResolution(int x, int y);
        int     GetWidth() { return _width; }
        int     GetHeight() { return _height; }

        float   ViewX;
        float   ViewY;
        float   ViewZ;

    private:
        int     _width;
        int     _height;
};
#define sGlobalVars ACE_Singleton<GlobalVars, ACE_Null_Mutex>::instance() ///- Der Crap ist singleton
#endif
