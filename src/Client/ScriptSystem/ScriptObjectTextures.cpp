#include "ScriptObject.hpp"
#include "TextureLoader.hpp"

///- Textures
int ScriptObject::Load2DMidmap(string &str)
{
    int tex = 0;
    TextureLoader *tload = new TextureLoader();
    tex = tload->Load2DMipmap(GetFilePath(str.c_str()));
    if (tex == 0)
        return 0;
    delete tload;
    return tex;
}

void ScriptObject::DeleteTexture(uint16 texture)
{
    GLuint tex = texture;
    glDeleteTextures(1, &tex);
} 

void ScriptObject::FlushTextures()
{
    uint32 vTex = 0;
    for (uint32 i = 0; i != MAX_TEXURES; i++)
    {
        if (glIsTexture(i))
        {
            _textures[vTex] = i;
            vTex++;
        }
    }
    glDeleteTextures(vTex, _textures);
}