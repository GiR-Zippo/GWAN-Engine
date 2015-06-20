/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "LoadingObject.hpp"
#include <math.h>

LoadingObject::LoadingObject(uint16 id, float x, float y, float z, float sx, float sy, uint16 tex, bool depth):
Object(id, x, y, z, sx, sy, tex, depth, false, false)
{
    _color->a = 0.4f;
    _color->r = 0.5f;
    _color->g = 0.5f;
    _color->b = 0.5f;
    _smax = 0;
    _open = false;
    _counter = 0;
}

void LoadingObject::Draw()
{
    // Enable depth testing if flag is set
    if(_depth)
    {
        glEnable(GL_DEPTH_TEST);
        glAlphaFunc (GL_GREATER, 0.1f);
        glEnable (GL_ALPHA_TEST);
    }
    else
        glDisable(GL_DEPTH_TEST);

    float zoom = _sx/2;
    float width = _sy;
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(_rotX, 1, 0, 0);
    glRotatef(_rotY, 0, 1, 0);
    glRotatef(_rotZ, 0, 0, 1);

    float z = -4;

    glPushMatrix(); //push the matrix so that our translations only affect
    {
        glTranslatef(_x, _y, z); //translate the tile to where it should belong
        glColor4f(_color->r,_color->g,_color->b,_color->a);
        glPointSize(1);
        //glColor3f(0.0,1.0,1.0);

        glBegin(GL_LINES);
        {
            int num_segments = 200;
            for(int r = 10; r < 20; r++)
                for(int ii = 0; ii < num_segments; ii++)
                {
                    bool found = false;
                    if (_counter > 490)
                    {
                        _counter = 0;
                        rot++;
                        if (_open)
                            _smax++;
                        else
                            _smax--;

                        if (_smax == 51)
                            _open = false;
                        else if (_smax == 0)
                            _open = true;
                    }
                    else
                    {
                        _counter++;
                    }
                    for (uint8 splint = 0; splint < _smax; splint++)
                    {
                        if (ii == 00+splint ||
                                ii == 50+splint ||
                                ii == 100+splint ||
                                ii == 150+splint)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        continue;


                    float theta = rot *2.0f * 3.1415926f * float(ii) / float(num_segments);  //get the current angle
                    float x = ((float)r/100) * cosf(theta);                             //calculate the x component
                    float y = ((float)r/100) * sinf(theta);                             //calculate the y component
                    glVertex2f(x + _x, y + _y);                                         //output vertex
                }
        }
        glEnd();
    }
    glGetFloatv (GL_MODELVIEW_MATRIX, _matrix);
    glPopMatrix(); //pop the matrix


    /*    glEnable(GL_TEXTURE_2D);
        {
            glEnable(GL_BLEND);
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBindTexture(GL_TEXTURE_2D, _texture); //get the texture for the actual quad
                glPushMatrix(); //push the matrix so that our translations only affect
                {
                    glTranslatef(_x, _y, z); //translate the tile to where it should belong
                    glColor4f(_Colr,_Colg,_Colb,_Cola);
                    glBegin(GL_QUADS); //begin drawing our quads
                    {
                        glTexCoord2d(0.0, 1.0);
                        glVertex2d((-1.0) * width, 1.0);

                        glTexCoord2d(1.0, 1.0);
                        glVertex2d((1.0) * width, 1.0);

                        glTexCoord2d(1.0, 0.0);
                        glVertex2d((1.0) * width, 0.9);

                        glTexCoord2d(0.0, 0.0);
                        glVertex2d((-1.0) * width, 0.9);
                    }
                    glEnd();
                }
                glGetFloatv (GL_MODELVIEW_MATRIX, _matrix);
                glPopMatrix(); //pop the matrix

                glPushMatrix(); //push the matrix so that our translations only affect
                {
                    glTranslatef(_x, _y, z);
                    glColor4f(1,1,1,1);
                    glBegin(GL_QUADS); //begin drawing our quads
                    {
                        float width = 3;
                        //TOP
                        glTexCoord2d(0.0, 1.0); glVertex2d(-1.0*width, 1.0);
                        glTexCoord2d(1.0, 1.0); glVertex2d(1.0*width, 1.0);
                        glTexCoord2d(1.0, 0.0); glVertex2d(1.0*width, 0.99);
                        glTexCoord2d(0.0, 0.0); glVertex2d(-1.0*width, 0.99);

                        //Bottom
                        glTexCoord2d(0.0, 1.0); glVertex2d(-1.0*width, 0.89);
                        glTexCoord2d(1.0, 1.0); glVertex2d(1.0*width, 0.89);
                        glTexCoord2d(1.0, 0.0); glVertex2d(1.0*width, 0.88);
                        glTexCoord2d(0.0, 0.0); glVertex2d(-1.0*width, 0.88);

                        //Left
                        glTexCoord2d(0.0, 1.0); glVertex2d(-1.0*width, 0.99);
                        glTexCoord2d(1.0, 1.0); glVertex2d(-0.99*width, 0.99);
                        glTexCoord2d(1.0, 0.0); glVertex2d(-0.99*width, 0.88);
                        glTexCoord2d(0.0, 0.0); glVertex2d(-1.0*width, 0.88);

                        //Right
                        glTexCoord2d(0.0, 1.0); glVertex2d(1.0*width, 0.99);
                        glTexCoord2d(1.0, 1.0); glVertex2d(0.99*width, 0.99);
                        glTexCoord2d(1.0, 0.0); glVertex2d(0.99*width, 0.88);
                        glTexCoord2d(0.0, 0.0); glVertex2d(1.0*width, 0.88);
                    }
                    glEnd();
                }
                glPopMatrix(); //pop the matrix
            }
            glDisable(GL_BLEND);
        }
        glDisable(GL_TEXTURE_2D);*/

    if(_depth)
    {
        glDisable (GL_ALPHA_TEST) ;
        glDisable(GL_DEPTH_TEST);
    }
}

