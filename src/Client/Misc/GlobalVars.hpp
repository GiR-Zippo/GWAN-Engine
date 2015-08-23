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
        void    SetStartupResolution(int x, int y);
        int     GetCurrentWidth() { return _currentwidth; }
        int     GetCurrentHeight() { return _currentheight; }
        int     GetStartupWidth() { return _width; }
        int     GetStartupHeight() { return _height; }

        float   ViewX;
        float   ViewY;
        float   ViewZ;

    private:
        int     _width;
        int     _height;
        int     _currentwidth;
        int     _currentheight;
};
#define sGlobalVars ACE_Singleton<GlobalVars, ACE_Null_Mutex>::instance() ///- Der Crap ist singleton
#endif
