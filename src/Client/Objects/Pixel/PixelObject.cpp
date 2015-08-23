/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "PixelObject.hpp"
#include "GlobalVars.hpp"
#include <math.h>

PixelObject::PixelObject(uint16 id, float x, float y, float z, float sx, float sy, uint16 tex, bool depth, bool ortho, bool clickable): 
Object(id, x, y, z, sx, sy, tex, depth, ortho, clickable)
{}

void PixelObject::Draw()
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

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(_rotX, 1, 0, 0);
    glRotatef(_rotY, 0, 1, 0);
    glRotatef(_rotZ, 0, 0, 1);
    glTranslatef(0.0f, 0.0f, 0.0f);

    float zoom, width;
    if (_ortho)
    {
        float w = (float)glutGet(GLUT_WINDOW_WIDTH) / sGlobalVars->GetStartupWidth();
        float h = (float)glutGet(GLUT_WINDOW_HEIGHT) / sGlobalVars->GetStartupHeight();
        glScalef(w, h, 1);
        pushScreenCoordinateMatrix();
    }
    else
    {
        zoom = _sx/2;
        width = _sy;   
    }

    glEnable(GL_TEXTURE_2D);
    {
        glEnable(GL_BLEND);
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, _texture); //get the texture for the actual quad
            glPushMatrix(); //push the matrix so that our translations only affect
            {
                glTranslatef(_x, _y, _z); //translate the tile to where it should belong
                glColor4f(_color->r,_color->g,_color->b,_color->a);
                glBegin(GL_QUADS); //begin drawing our quads
                {
                    if (_ortho)
                    {
                        glTexCoord2d(0.0, 1.0);
                        glVertex2d(0, _sy);

                        glTexCoord2d(1.0, 1.0);
                        glVertex2d(_sx, _sy);

                        glTexCoord2d(1.0, 0.0);
                        glVertex2d(_sx, 0);
                        
                        glTexCoord2d(0.0, 0.0);
                        glVertex2d(0, 0);
                    }
                    else
                    {
                        glTexCoord2d(0.0, 1.0);
                        glVertex2d((-1.0 * zoom) * width, 1.0 * zoom);

                        glTexCoord2d(1.0, 1.0);
                        glVertex2d((1.0 * zoom) * width, 1.0 * zoom);

                        glTexCoord2d(1.0, 0.0);
                        glVertex2d((1.0 * zoom) * width, -1.0 * zoom);
                        
                        glTexCoord2d(0.0, 0.0);
                        glVertex2d((-1.0 * zoom) * width, -1.0 * zoom);
                    }

                }
                glEnd();
            }
            glGetFloatv (GL_MODELVIEW_MATRIX, _matrix);
            glPopMatrix(); //pop the matrix
        }
        glDisable(GL_BLEND);
    }

    if (_ortho)
        pop_projection_matrix();
    
    if(_depth)
    {
        glDisable (GL_ALPHA_TEST) ;
        glDisable(GL_DEPTH_TEST);
    }
    glDisable(GL_TEXTURE_2D);
}
