#ifndef __SHADERLOADER_HPP__
#define __SHADERLOADER_HPP__

#include "Common.h"

class ShaderLoader
{
    public:
        GLuint LoadShaderFile(const char *filename, GLenum type);
        void CheckLinkError(GLuint prog);
        void PrintProgramInfoLog(GLuint obj);
};
#endif
