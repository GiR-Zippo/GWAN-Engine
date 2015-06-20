/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __WRAPPER_H_
#define __WRAPPER_H_

#include "Common.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

class Wrapper
{
    public:
        virtual void Init();
        virtual void InitGL();
        virtual void Draw();
        virtual void Idle();
        virtual void KeyBoardSpecial(int key, int xmouse, int ymouse);
        virtual void KeyBoardNormal(unsigned char key, int xmouse, int ymouse);
        virtual void Tick(uint32 diff);
        virtual void Resize(int w, int h);
        virtual void Motion(int x, int y);
        virtual void Mouse(int button, int state, int x, int y);

        int _tileRef[257];
};
#endif