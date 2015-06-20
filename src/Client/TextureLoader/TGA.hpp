#ifndef __TGA_HPP__
#define __TGA_HPP__

#include "Common.h"
#include <vector>

class TextureLoader;
class RGBA
{
    public:
        uint32 Colour;
        uint8 B, G, R, A;
};


class Tga
{
    public:
        Tga(TextureLoader &in, const char* FilePath);
        uint32 GetWidth() { return width; }
        uint32 GetHeight() { return height; }
        uint32 GetSize() { return size; }
        
    private:
        std::vector<unsigned char> ImageData;
        bool ImageCompressed;
        uint32 width, height, size, BitsPerPixel;
        TextureLoader &_texloader;
}; 

#endif