#include "ScriptObject.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "GlobalVars.hpp"

int ScriptObject::CreateShader(std::string filename)
{
    Shader *shader = new Shader(filename);
    _shader[shader->GetProgramm()] = shader;
    return shader->GetProgramm();
}

void ScriptObject::SetObjectShader(uint16 id, uint16 shader)
{
    if(_objectList[id])
        _objectList[id]->SetShader(_shader[shader]);
}

void ScriptObject::SetUniform1f(uint16 id, std::string name, float valA)
{
    _shader[id]->SetUniform1f(name, valA);
}

void ScriptObject::SetUniform2f(uint16 id, std::string name, float valA, float valB)
{
    _shader[id]->SetUniform2f(name, valA, valB);
}

void ScriptObject::DeleteShader(uint16 id)
{
    delete _shader[id];
    _shader[id] = NULL;
}

void ScriptObject::FlushShaders()
{
    for (int i = 0; i < MAX_SHADERS; i++)
        _shader[i] = NULL;
}