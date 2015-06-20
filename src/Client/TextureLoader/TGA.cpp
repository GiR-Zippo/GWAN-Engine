#include "TGA.hpp"
#include "TextureLoader.hpp"

Tga::Tga(TextureLoader& in, const char* FilePath) : _texloader(in)
{
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);

    if (!hFile.is_open())
    {
        std::cout << "File Not Found.\n";
        return;
    }

    uint8 Header[18] = {0};
    static uint8 DeCompressed[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static uint8 IsCompressed[12] = {0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    hFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));

    BitsPerPixel = Header[16];
    width  = (Header[13] * 0xFF + Header[12])+1;
    height = (Header[15] * 0xFF + Header[14])-1;
    size  = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    _texloader._width = width;
    _texloader._height = height;

    if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
    {
        hFile.close();
        std::cout << "Invalid File Format. Required: 24 or 32 Bit Image.\n";
        return;
    }

    _texloader._type = GL_RGBA;
    if (BitsPerPixel == 24)
        _texloader._type = GL_RGB;

    if (!memcmp(DeCompressed, &Header, sizeof(DeCompressed)))
    {
        ImageCompressed = false;
        _texloader._image_data = new png_byte[size];
        hFile.read(reinterpret_cast<char*>(_texloader._image_data ), size);
        GLuint cswap;
        for(cswap = 0; cswap < (int)size; cswap += BitsPerPixel/8)
            _texloader._image_data[cswap] ^= _texloader._image_data[cswap+2] ^= _texloader._image_data[cswap] ^= _texloader._image_data[cswap+2];
    }
/*    else if (!memcmp(IsCompressed, &Header, sizeof(IsCompressed)))
    {
        RGBA *Pixel = new RGBA[width * height];
        int CurrentByte = 0;
        std::size_t CurrentPixel = 0;
        ImageCompressed = true;
        uint8 ChunkHeader = {0};
        int BytesPerPixel = (BitsPerPixel / 8);
        _texloader._image_data = new png_byte[width * height * sizeof(RGBA)];

        do
        {
            hFile.read(reinterpret_cast<char*>(&ChunkHeader), sizeof(ChunkHeader));

            if (ChunkHeader < 128)
            {
                ++ChunkHeader;

                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
                {
                    hFile.read(reinterpret_cast<char*>(Pixel), BytesPerPixel);

                    _texloader._image_data[CurrentByte++] = Pixel->B;
                    _texloader._image_data[CurrentByte++] = Pixel->G;
                    _texloader._image_data[CurrentByte++] = Pixel->R;

                    if (_texloader._type == GL_RGBA)
                        _texloader._image_data[CurrentByte++] = Pixel->A;
                }
            }
            else
            {
                ChunkHeader -= 127;
                hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
                {
                    _texloader._image_data[CurrentByte++] = Pixel->B;
                    _texloader._image_data[CurrentByte++] = Pixel->G;
                    _texloader._image_data[CurrentByte++] = Pixel->R;

                    if (_texloader._type == GL_RGBA)
                        _texloader._image_data[CurrentByte++] = Pixel->A;
                }
            }
        }
        while (CurrentPixel < (width * height));
    }*/
    else
    {
        hFile.close();
        std::cout << "Invalid File Format. Required: 24 or 32 Bit Image.\n";
        return;
    }

    hFile.close();
    //ImageData.data();
}
