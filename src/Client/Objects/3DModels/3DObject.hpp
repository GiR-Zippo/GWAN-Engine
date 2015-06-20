/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __3DOBJECT_HPP__
#define __3DOBJECT_HPP__

#include "Object.hpp"
#include "3DModel.hpp"

enum DreiType
{
    DT_MILKSHAPE = 0,
    DT_WAVEFRONT = 1
};

class Model;
class DreiDObject : public Object
{
    public:
        DreiDObject(uint16 id, float x, float y, float z);
        ~DreiDObject();
        void Draw();

        //Wavefront (obj)
        bool LoadObjModelData(const char *path, const char *filename);
        //Milkshape
        bool LoadMilkShapeModelData(const char *path, const char *filename);
        void SetPath(std::string path) { _path = path; }
        std::string _path;
    private:
        uint8 m_Type;
        Model *m_Model;
        uint32 _CallListNum;
};
#endif
