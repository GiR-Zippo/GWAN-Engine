#include "ShaderLoader.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

/// Print out the information log for a shader object
/// @arg obj handle for a program object
void ShaderLoader::PrintProgramInfoLog(GLuint obj)
{
    GLint infologLength = 0, charsWritten = 0;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 2)
    {
        GLchar* infoLog = new GLchar [infologLength];
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        std::cerr << infoLog << std::endl;
        delete infoLog;
    }
}

GLuint ShaderLoader::LoadShaderFile(const char* filename, GLenum type)
{
    GLuint shaderID = glCreateShader(type);
    ifstream::pos_type size;
    char* source;
    string text;

    ifstream file (filename, ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        source = new char[1 + size];
        file.seekg (0, ios::beg);
        file.read (source, size);
        file.close();
        source[size] = '\0';
        cout << "file " << filename << " loaded" << endl;
        text.assign(source);
    }
    else
    {
        cout << "Unable to open file " << filename << endl;
        exit(1);
    }

    glShaderSource(shaderID, 1, (const GLchar**)&source, NULL);
    glCompileShader(shaderID);

    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        cerr << "Failed to compile shader from \"" << filename << "\"" << endl;
        GLchar log[16384];
        glGetShaderInfoLog(shaderID, 16384, NULL, log);
        cerr << "Log following:" << endl;
        cerr << log << endl;
        exit(1);
    }

    return shaderID;
}

void ShaderLoader::CheckLinkError(GLuint prog)
{
    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        cerr << "Failed to link shaders:" << endl;
        GLchar log[16384];
        glGetProgramInfoLog(prog, 16384, NULL, log);
        cerr << "Log following:" << endl;
        cerr << log << endl;
        exit(1);
    }
}
