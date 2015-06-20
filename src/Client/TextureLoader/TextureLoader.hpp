/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __TEXURELOADER_HPP__
#define __TEXTURELOADER_HPP__

#include "Common.h"

#define PNGLCONF_H
#include "png.h"

class Tga;
class TextureLoader
{
    friend Tga;
    public:
        ~TextureLoader();
        int LoadTexture(std::string filename);
        int Load2DMipmap(std::string filename);
        std::map<uint32, GLuint> LoadTiledTexture(const std::string filename, int tilewidth, int tileheight);

    private:
        int LoadPNGFile(std::string filename);
        int LoadTGAFile(std::string filename);
        int LoadBMPFile(std::string filename);

    private:
        int         _response;
        uint32      _width;
        uint32      _height;

        FILE       *_filepointer;
        png_structp _png_ptr;
        png_infop   _info_ptr;
        png_infop   _end_info;
        png_bytep  *_row_pointers;
        png_byte   *_image_data;
        GLuint      _type;  
};
#endif
