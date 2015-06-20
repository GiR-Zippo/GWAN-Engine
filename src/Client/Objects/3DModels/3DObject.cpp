/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "3DObject.hpp"
#include "MS3D.hpp"
#include "OBJ.hpp"
DreiDObject::DreiDObject(uint16 id, float x, float y, float z):
Object(id, x, y, z, 0, 0, 0, false, false, false)
{}

DreiDObject::~DreiDObject()
{

}

void DreiDObject::Draw()
{
    glLoadIdentity();
    glTranslatef(_x, _y, _z);
    glRotatef(_rotX, 1, 0, 0);
    glRotatef(_rotY, 0, 1, 0);
    glRotatef(_rotZ, 0, 0, 1);

    bool bCull =true;
    bool bDepth = true;
    bool bOutline = false;
    // Turn culling on if flag is set
    if(bCull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);


    // Enable depth testing if flag is set
    if(bDepth)
    {
        glEnable(GL_DEPTH_TEST);
        glAlphaFunc (GL_GREATER, 0.1f);
        glEnable (GL_ALPHA_TEST);
    }
    else
        glDisable(GL_DEPTH_TEST);

    // Draw the back side as a wireframe only, if flag is set
    if(bOutline)
        glPolygonMode(GL_BACK,GL_LINE);
    else
        glPolygonMode(GL_BACK,GL_FILL);

    //if (m_Type == DT_MILKSHAPE)
    glColor4f(_color->r,_color->g,_color->b,_color->a);
    glCallList(_CallListNum);

    // Turn culling on if flag is set
    if(bCull)
        glDisable(GL_CULL_FACE);

    // Enable depth testing if flag is set
    if(bDepth)
    {
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_DEPTH_TEST);
    }

    // Draw the back side as a wireframe only, if flag is set
    if(bOutline)
        glPolygonMode(GL_BACK,GL_FILL);
    else
        glPolygonMode(GL_BACK,GL_LINE);
}

bool DreiDObject::LoadObjModelData(const char *path, const char *filename)
{
    m_Type = DT_WAVEFRONT;
    m_Model = new OBJModel;
    _CallListNum = m_Model->LoadModelData(path, filename);
    delete m_Model;
    return true;
}

bool DreiDObject::LoadMilkShapeModelData(const char *path, const char *filename)
{
    m_Type = DT_MILKSHAPE;
    m_Model = new MS3D;
    _CallListNum = m_Model->LoadModelData(path, filename);
    delete m_Model;
    return true;
}
