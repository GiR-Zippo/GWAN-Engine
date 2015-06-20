/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __TEXTOBJECT_HPP__
#define __TEXTOBJECT_HPP__

#include "Object.hpp"
#include <ft2build.h>
#include <ftglyph.h>
//#include <freetype.h>
#include "freetype.h"                                                                           // Header For Our Little Font Library.
#include <vector>
#include <string>

class TextObject : public Object
{
    public:
        TextObject(uint16 id, float x, float y, float z, bool depth, float height, std::string text, std::string font);
        ~TextObject();
        void Draw();
        void PostDraw();
        void SetText(std::string text) { _text = text; }
        void SetCenterX(bool arg) { _centerX = arg; }
        void SetCenterY(bool arg) { _centerY = arg; }

        bool IsMouseHit(float x, float y);
    private:
        // The Init Function Will Create A Font With
        // The Height h From The File fname.
        void init(const char * fname);

        // Free All The Resources Associated With The Font.
        void clean();
        
        void make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base );
        
        void print(float x, float y, const char *fmt, ...);
    private:
        bool _isInitialized;
        bool _centerX;
        bool _centerY;
        bool _newTextColor;
        std::string _fontFile;
        std::string _text;
        float _textlen;
        uint8 _height;      // Holds The Height Of The Font.
        GLuint * textures;  // Holds The Texture Id's
        GLuint list_base;   // Holds The First Display List Id
};
#endif
 
 
