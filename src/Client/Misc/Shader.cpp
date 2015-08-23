#include "Shader.hpp"
#include "ShaderLoader.hpp"
#include "GlobalVars.hpp"
#include "Common.h"

Shader::Shader(string filename)
{
    ShaderLoader *loader = new ShaderLoader();
    _VS = loader->LoadShaderFile(GetFilePath(filename + ".vs").c_str(), GL_VERTEX_SHADER);
    _FS = loader->LoadShaderFile(GetFilePath(filename + ".fs").c_str(), GL_FRAGMENT_SHADER);
    _PO = glCreateProgram();
    glAttachShader(_PO, _VS);
    loader->PrintProgramInfoLog(_PO);
    glAttachShader(_PO, _FS);
    loader->PrintProgramInfoLog(_PO);
    glLinkProgram(_PO);
    loader->CheckLinkError(_PO);
    delete loader;
}

Shader::~Shader()
{
    glDeleteShader(_FS);
    glDeleteShader(_VS);
    glDeleteShader(_PO);
}

void Shader::Update()
{
    glUseProgram(_PO);
    while (!_Uniform1fStack.empty())
    {
        Uniform1f *data = _Uniform1fStack.top();
        _Uniform1fStack.pop();
        glUniform1f(glGetUniformLocation(_PO, data->name.c_str()),  data->valA);
        delete data;
    }
    while (!_Uniform2fStack.empty())
    {
        Uniform2f *data = _Uniform2fStack.top();
        _Uniform2fStack.pop();
        glUniform2f(glGetUniformLocation(_PO, data->name.c_str()),  data->valA, data->valB);
        delete data;
    }
}

void Shader::Finish()
{
    glUseProgram(0);
}

void Shader::SetUniform1f(std::string name, float valA)
{
    Uniform1f *data = new Uniform1f;
    data->name = name;
    data->valA = valA;
    _Uniform1fStack.push(data);
}

void Shader::SetUniform2f(std::string name, float valA, float valB)
{
    Uniform2f *data = new Uniform2f;
    data->name = name;
    data->valA = valA;
    data->valB = valB;
    _Uniform2fStack.push(data);
}