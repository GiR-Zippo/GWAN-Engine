/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Object.hpp"
#include "GlobalVars.hpp"
#include <math.h>

Object::Object(uint16 id, float x, float y, float z, float sx, float sy, uint16 tex, bool depth, bool ortho, bool clickable)
{
    _id = id;
    _color = new RGBA();
    _color->a = 1.0f;
    _color->r = 1.0f;
    _color->g = 1.0f;
    _color->b = 1.0f;
    _x = x;
    _y = y;
    _z = z;
    _sx = sx;
    _sy = sy;
    _texture = tex;
    _depth = depth;
    _ortho = ortho;
    _click = clickable;

    _rotX = 0;
    _rotY = 0;
    _rotZ = 0;
}

Object::~Object()
{}

void Object::Draw()
{}

void Object::PostDraw()
{}

void Object::KeyPressed(uint8 key)
{}

void Object::SetObjectRGBA(float r, float g, float b, float a)
{
    if (r != -1)
        _color->r = r;
    if (g != -1)
        _color->g = g;
    if (b != -1)
        _color->b = b;
    if (a != -1)
        _color->a = a;
}

bool Object::IsMouseHit(float x, float y)
{    
    if (!_ortho)
        return false;
    
    if (!_click)
        return false;

    glLoadIdentity();
    float w = (float)glutGet(GLUT_WINDOW_WIDTH) / sGlobalVars->GetWidth();
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT) / sGlobalVars->GetHeight();
    float my = -(y - (float)glutGet(GLUT_WINDOW_HEIGHT));
    if ((x >= _x*w && x <= (_sx+_x)*w) && ((my >= _y*h) && my <= (_sy+_y) *h))
        return true;
    return false;
}

void Object::pushScreenCoordinateMatrix()
{
    glPushAttrib(GL_TRANSFORM_BIT);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
    glPopAttrib();
}

void Object::pop_projection_matrix()
{
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}