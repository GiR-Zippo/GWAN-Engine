/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __LAUNCHER_HPP__
#define __LAUNCHER_HPP__

#include "wrapper.h"
#include "angelscript.h"
#include "scriptmgr.h"

#include "BackgroundDownloadSystem.hpp"

class Sound;

class ScriptObject;

class Launcher: public Wrapper
{
    public:
        Launcher();
        ~Launcher();
        void Init();
        void InitGL();
        void Draw();
        void Idle();
        void Tick(uint32 diff);
        void KeyBoardNormal(unsigned char key, int xmouse, int ymouse);
        void Resize(int w, int h);
        void Mouse(int button, int state, int x, int y);
        void Motion(int x, int y);

        void LoadScript(std::string file);
        
        void CheckFileOrDownload(std::string filename, bool blocking = false);
        void LoadFileFromServer(std::string filename, bool blocking = false);
        bool IsLastFile(std::string file);
        
        ///- Var-Container
        void SetVarCon(std::string index, std::string data);
        std::string GetStringFromVarCon(std::string index);
        float GetFloatFromVarCon(std::string index);
        uint32 GetUint32FromVarCon(std::string index);
        
        ///- Directory commands
        void mkdir(std::string dir);
        ///- BackgroundDownload
        void Download(std::string ip, uint16 port, std::string filename);
        uint64 GetActualBlock();
        uint64 GetTotalBlocks();
        
        void SetViewXYZ(float x, float y, float z) { _tviewx = x; _tviewy =y; _tviewz =z;}
    private:
        ACE_Based::Thread BGDLMGR;
        ScriptObject *_script;
        CScriptMgr *scriptMgr;
        float _tviewx;
        float _tviewy;
        float _tviewz;
        float _rotx;
        float _roty;
        float _rotz;
        float px;
        float py;
        bool _scriptInitialized;
        
        std::map<std::string, std::string> _varMap; /* Die map speichert Variablen zwischen den Scripts */
        Sound *_sound;
        
        GLuint program;
};
#endif
