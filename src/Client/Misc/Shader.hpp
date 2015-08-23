#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include "Common.h"
#include <stack>

struct Uniform1f
{
    std::string name;
    float valA;
};

struct Uniform2f
{
    std::string name;
    float valA;
    float valB;
};

class Shader
{
    public:
        Shader(std::string filename);
        ~Shader();
        void Update();
        void Finish();

        uint32 GetProgramm() { return _PO; }
        
        void SetUniform1f(std::string name, float valA);
        void SetUniform2f(std::string name, float valA, float valB);
        float timer;

    private:
        std::stack<Uniform1f*> _Uniform1fStack;
        std::stack<Uniform2f*> _Uniform2fStack;
        GLuint _VS;
        GLuint _FS;
        GLuint _PO;
};
#endif
