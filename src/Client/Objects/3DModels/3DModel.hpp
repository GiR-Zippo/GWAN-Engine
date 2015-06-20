/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __3DMODEL_HPP__
#define __3DMODEL_HPP__

#include "Common.h"

class Model
{
    public:
        virtual int LoadModelData(const char *path, const char *filename) { return false; }
};
#endif
