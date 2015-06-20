/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __TEXTBOX_HPP__
#define __TEXTBOX_HPP__

#include "Object.hpp"
#include "TextureLoader.hpp"
#include <ft2build.h>
#include <ftglyph.h>
#include <freetype.h>
#include <vector>
#include <string>

class Textbox : public Object
{
    public:
        Textbox(uint16 id, float x, float y, float z, bool depth, float height, std::string text, std::string font);
        ~Textbox();
        void Draw();
        void PostDraw();
        void SetText(std::string text) { _text = text; }
        void SetTextRGBA(float r, float g, float b, float a);
        void SetbackgroundTexture(std::string textureName);
        
        //Interaktionen
        void SetReturnKeyOn(uint8 key);

    private:
        // The Init Function Will Create A Font With
        // The Height h From The File fname.
        void init(const char * fname);

        // Free All The Resources Associated With The Font.
        void clean();
        
        void make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base );
        
        void print(float x, float y, const char *fmt, ...);
    private:
        bool _isInteractive;
        bool _isInitialized;
        bool _loadBackgroundTexture;
        std::string _backgroundTextureName;
        std::string _fontFile;
        std::string _text;
        uint8 _height;      // Holds The Height Of The Font.
        GLuint * textures;  // Holds The Texture Id's
        GLuint list_base;   // Holds The First Display List Id
};
#endif
