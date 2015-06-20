/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "TextureLoader.hpp"

#include "TGA.hpp"
#include "wrapper.h"

#define TEXTURE_LOAD_ERROR 0

bool has_suffix(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

TextureLoader::~TextureLoader()
{
    png_destroy_read_struct(&_png_ptr, &_info_ptr, &_end_info);
    delete[] _image_data;
    delete[] _row_pointers;

    if (_filepointer)
        fclose(_filepointer);
}

int TextureLoader::LoadTexture(string filename)
{
    if (has_suffix(filename, ".png") || has_suffix(filename, ".PNG"))
        if (LoadPNGFile(filename) == TEXTURE_LOAD_ERROR)
            return TEXTURE_LOAD_ERROR;

    if (has_suffix(filename, ".tga") || has_suffix(filename, ".TGA"))
        if (LoadTGAFile(filename) == TEXTURE_LOAD_ERROR)
            return TEXTURE_LOAD_ERROR;


    GLuint id;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    if (_type == GL_RGBA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image_data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glDisable(GL_TEXTURE_2D);
    return id;
}

int TextureLoader::Load2DMipmap(std::string filename)
{
    if (has_suffix(filename, ".png") || has_suffix(filename, ".PNG"))
        if (LoadPNGFile(filename) == TEXTURE_LOAD_ERROR)
            return TEXTURE_LOAD_ERROR;

    if (has_suffix(filename, ".tga") || has_suffix(filename, ".TGA"))
        if (LoadTGAFile(filename) == TEXTURE_LOAD_ERROR)
            return TEXTURE_LOAD_ERROR;

    GLuint id;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    if (_type == GL_RGBA)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _image_data);

    else
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, _image_data);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glEnable (GL_TEXTURE_GEN_S);
    glEnable (GL_TEXTURE_GEN_T);
    glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glDisable (GL_TEXTURE_GEN_T);
    glDisable (GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_2D);
    return id;
}

std::map<uint32, GLuint> TextureLoader::LoadTiledTexture(const std::string filename, int tilewidth, int tileheight)
{
    std::map<uint32, GLuint> textureSet;

    if (LoadPNGFile(filename) == TEXTURE_LOAD_ERROR)
    {
        textureSet[0] = (TEXTURE_LOAD_ERROR);
        return textureSet;
    }

    //Now generate the OpenGL texture object
    int st_x = _width / tilewidth;
    int st_y = (_height / tileheight); //Yep das gibt nen Countdown
    GLuint texture;

    uint32 count = 0;

    for (int y = st_y - 1; y >= 0; y--)
    {
        for (int x = 0; x < st_x; x++)
        {
            glPixelStorei(GL_UNPACK_ROW_LENGTH, _width);
            glPixelStorei(GL_UNPACK_SKIP_ROWS, y * tileheight);
            glPixelStorei(GL_UNPACK_SKIP_PIXELS, x * tilewidth);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glEnable(GL_TEXTURE_2D);
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tilewidth, tileheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image_data);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            textureSet[count] = texture;
            count++;
            glDisable(GL_TEXTURE_2D);
        }
    }

    //clean up memory and close stuff
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
    return textureSet;
}

int TextureLoader::LoadPNGFile(std::string filename)
{

    _type = GL_RGBA;
    //header for testing if it is a png
    png_byte header[8];

    //open file as binary
    _filepointer = fopen(filename.c_str(), "rb");

    if (!_filepointer)
    {
        return TEXTURE_LOAD_ERROR;
    }

    //read the header
    size_t t = fread(header, 1, 8, _filepointer);

    //test if png
    int is_png = !png_sig_cmp(header, 0, 8);

    if (!is_png)
    {
        fclose(_filepointer);
        return TEXTURE_LOAD_ERROR;
    }

    //create png struct
    _png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!_png_ptr)
    {
        fclose(_filepointer);
        return (TEXTURE_LOAD_ERROR);
    }

    //create png info struct
    _info_ptr = png_create_info_struct(_png_ptr);

    if (!_info_ptr)
    {
        png_destroy_read_struct(&_png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(_filepointer);
        return (TEXTURE_LOAD_ERROR);
    }

    //create png info struct
    _end_info = png_create_info_struct(_png_ptr);

    if (!_end_info)
    {
        png_destroy_read_struct(&_png_ptr, &_info_ptr, (png_infopp) NULL);
        fclose(_filepointer);
        return (TEXTURE_LOAD_ERROR);
    }

    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(_png_ptr)))
    {
        png_destroy_read_struct(&_png_ptr, &_info_ptr, &_end_info);
        fclose(_filepointer);
        return (TEXTURE_LOAD_ERROR);
    }

    //init png reading
    png_init_io(_png_ptr, _filepointer);

    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(_png_ptr, 8);

    // read all the info up to the image data
    png_read_info(_png_ptr, _info_ptr);

    //variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 twidth, theight;

    // get info about png
    png_get_IHDR(_png_ptr, _info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

    //update width and height based on png info
    _width = twidth;
    _height = theight;

    // Update the png info struct.
    png_read_update_info(_png_ptr, _info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(_png_ptr, _info_ptr);

    // Allocate the image_data as a big block, to be given to opengl
    _image_data = new png_byte[rowbytes * _height];

    if (!_image_data)
    {
        //clean up memory and close stuff
        png_destroy_read_struct(&_png_ptr, &_info_ptr, &_end_info);
        fclose(_filepointer);
        return TEXTURE_LOAD_ERROR;
    }

    //row_pointers is for pointing to image_data for reading the png with libpng
    _row_pointers = new png_bytep[_height];

    if (!_row_pointers)
    {
        //clean up memory and close stuff
        png_destroy_read_struct(&_png_ptr, &_info_ptr, &_end_info);
        delete[] _image_data;
        fclose(_filepointer);
        return TEXTURE_LOAD_ERROR;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (uint64 i = 0; i < _height; ++i)
        _row_pointers[_height - 1 - i] = _image_data + i * rowbytes;

    //read the png into image_data through row_pointers
    png_read_image(_png_ptr, _row_pointers);
    return 1;
}

int TextureLoader::LoadTGAFile(std::string filename)
{
    Tga* tga = new Tga(*this, filename.c_str());

    if (_image_data)
    {
        delete tga;
        return 1;
    }

    delete tga;
    return TEXTURE_LOAD_ERROR;
}

int TextureLoader::LoadBMPFile(std::string filename)
{
    unsigned short int bfType;
    long int bfOffBits;
    short int biPlanes;
    short int biBitCount;
    long int biSizeImage;
    int i;
    unsigned char temp;

    /* make sure the file is there and open it read-only (binary) */
    if ((_filepointer = fopen(filename.c_str(), "rb")) == NULL)
    {
        printf("File not found : %s\n", filename.c_str());
        return TEXTURE_LOAD_ERROR;
    }

    if (!fread(&bfType, sizeof(short int), 1, _filepointer))
    {
        printf("Error reading file!\n");
        return TEXTURE_LOAD_ERROR;
    }

    /* check if file is a bitmap */
    if (bfType != 19778)
    {
        printf("Not a Bitmap-File!\n");
        return TEXTURE_LOAD_ERROR;
    }

    /* get the file size */
    /* skip file size and reserved fields of bitmap file header */
    fseek(_filepointer, 8, SEEK_CUR);

    /* get the position of the actual bitmap data */
    if (!fread(&bfOffBits, sizeof(long int), 1, _filepointer))
    {
        printf("Error reading file!\n");
        return TEXTURE_LOAD_ERROR;
    }

    printf("Data at Offset: %ld\n", bfOffBits);
    /* skip size of bitmap info header */
    fseek(_filepointer, 4, SEEK_CUR);
    /* get the width of the bitmap */
    fread(&_width, sizeof(int), 1, _filepointer);
    printf("Width of Bitmap: %d\n", _width);
    /* get the height of the bitmap */
    fread(&_height, sizeof(int), 1, _filepointer);
    printf("Height of Bitmap: %d\n", _height);
    /* get the number of planes (must be set to 1) */
    fread(&biPlanes, sizeof(short int), 1, _filepointer);

    if (biPlanes != 1)
    {
        printf("Error: number of Planes not 1!\n");
        return TEXTURE_LOAD_ERROR;
    }

    /* get the number of bits per pixel */
    if (!fread(&biBitCount, sizeof(short int), 1, _filepointer))
    {
        printf("Error reading file!\n");
        return TEXTURE_LOAD_ERROR;
    }

    printf("Bits per Pixel: %d\n", biBitCount);

    if (biBitCount != 24)
    {
        printf("Bits per Pixel not 24\n");
        return TEXTURE_LOAD_ERROR;
    }

    /* calculate the size of the image in bytes */
    biSizeImage = _width * _height * 3;
    printf("Size of the image data: %ld\n", biSizeImage);
    //texture->data = malloc(biSizeImage);
    _image_data = new png_byte[_width * _height];
    /* seek to the actual data */
    fseek(_filepointer, bfOffBits, SEEK_SET);

    if (!fread(_image_data, biSizeImage, 1, _filepointer))
    {
        printf("Error loading file!\n");
        return TEXTURE_LOAD_ERROR;
    }

    /* swap red and blue (bgr -> rgb) */
    for (i = 0; i < biSizeImage; i += 3)
    {
        temp = _image_data[i];
        _image_data[i] = _image_data[i + 2];
        _image_data [i + 2] = temp;
    }

    return 1;
}
