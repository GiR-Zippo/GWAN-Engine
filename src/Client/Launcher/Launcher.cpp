/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Launcher.hpp"
#include "ScriptObject.hpp"
#include "DownloadSystem.hpp"

#include "Sound.hpp"
#include "SoundMgr.hpp"
#include "ObjectMgr.hpp"

#include "GlobalVars.hpp"

Launcher::Launcher()
{}

Launcher::~Launcher()
{
    BGDLMGR.destroy();
}

void Launcher::Init()
{
    sGlobalVars->ViewX = 0.0f;
    sGlobalVars->ViewY = 0.0f;
    sGlobalVars->ViewZ = 0.0f;
    _rotx = 0.0f;
    _roty = 0.0f;
    _rotz = 0.0f;

    //Temp
    py = 0;
    px = 1.0;
    LoadFileFromServer("main.as", true);
    _scriptInitialized = false;
    _script = new ScriptObject(GetFilePath("main"));
    scriptMgr = new CScriptMgr();

    int r = scriptMgr->Init(this);
    _script->SetScriptObject(scriptMgr->CreateController(GetFilePath("main"), _script), scriptMgr);
    _scriptInitialized = true;
}

void Launcher::LoadScript(std::string file)
{
    _scriptInitialized = false;
    std::string old = _script->GetScriptName();
    _script->FlushTextures();
    _script->DestroyAndRelease();
    scriptMgr->RemoveScript(old);
    _script = new ScriptObject(GetFilePath(file));
    _script->SetScriptObject(scriptMgr->CreateController(GetFilePath(file), _script), scriptMgr);
    _script->InitGL();
    _scriptInitialized = true;
}

void Launcher::InitGL()
{
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 10.f);
    glFogf(GL_FOG_END, 40.f);
    glFogf(GL_FOG_DENSITY, 100);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    
    // Create light components 
    GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
    GLfloat specularLight[] = {1.5f, 0.5f, 1.5f, 0.5f };
    GLfloat position[] = { 0.0f, 0.5f, 10.0f, 1.0f };
    
    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHT0);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    gluPerspective (1, (GLfloat)sGlobalVars->GetStartupWidth() / (GLfloat)sGlobalVars->GetStartupHeight(), 1.0, 1000.0);
    if (_scriptInitialized && _script)
        _script->InitGL();
}

void Launcher::Draw()
{
    ///- Kamera für die Scene
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);

    if (_tviewx !=0 || _tviewy !=0 || _tviewz !=0)
    {
        glTranslatef(_tviewx, _tviewy, _tviewz);
        sGlobalVars->ViewX += _tviewx;
        sGlobalVars->ViewY += _tviewy;
        sGlobalVars->ViewZ += _tviewz;

        _tviewx = 0;
        _tviewy = 0;
        _tviewz = 0;
        _rotx = 0;
        _roty = 0;
        _rotz = 0;
    }
    if (_rotx)
    {
        glRotatef(_rotx, 1, 0 ,0);
        _rotx = 0;
    }
    if (_roty)
    {
        glRotatef(_roty, 0, 1 ,0);
        _roty = 0;
    }
    if (_rotz)
    {
        glRotatef(_rotz, 0, 0 ,1);
        _rotz = 0;
    }
    
    ///- Der ganze RenderKram
    glClearColor (0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    gluLookAt (0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0f);

    if (_scriptInitialized && _script)
        _script->Draw();

    glutSwapBuffers();
    if (_scriptInitialized && _script)
        _script->PostDraw();
}

void Launcher::Idle()
{
    glutPostRedisplay();
    #ifdef WIN32
        Sleep(10);
    #else
        usleep(10000);   // usleep takes sleep time in us
    #endif
}

void Launcher::Tick(uint32 diff)
{
    sSoundMgr->Update();

    if (_scriptInitialized && _script)
        _script->Tick(diff);
}

void Launcher::Resize(int w, int h)
{
    sGlobalVars->SetResolution(w,h);
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glTranslatef(sGlobalVars->ViewX, sGlobalVars->ViewY, sGlobalVars->ViewZ);
    glMatrixMode (GL_MODELVIEW);
    if (_scriptInitialized && _script)
        _script->OnResize(w, h);
}

void Launcher::KeyBoardNormal(unsigned char key, int xmouse, int ymouse)
{
    if (_scriptInitialized && _script)
        _script->KeyPressed(key);
    switch (key)
    {
        case '0': //Reload
            sObjectMgr->ClearList();
            _scriptInitialized = false;
            std::string old = _script->GetScriptName();
            _script->FlushTextures();
            _script->DestroyAndRelease();
            scriptMgr->RemoveScript(old);
            _script = new ScriptObject(old);
            _script->SetScriptObject(scriptMgr->CreateController(old, _script), scriptMgr);
            _script->InitGL();
            _scriptInitialized = true;
            break;
    };
}

void Launcher::Mouse(int button, int state, int x, int y)
{
    if (_scriptInitialized && _script)
        _script->Mouse(button, state, x, y);
}

void Launcher::Motion(int x, int y)
{
    if (_scriptInitialized && _script)
        _script->MouseMove(x, y);
}

//FileIO
void Launcher::CheckFileOrDownload(string filename, bool blocking)
{
    if (blocking)
        sDownloadSystem->AddToBlockingQueue(filename);
    else
        sDownloadSystem->AddToNonBlockingQueue(filename);
}

void Launcher::LoadFileFromServer(string filename, bool blocking)
{
    if (blocking)
        sDownloadSystem->AddToBlockingQueue(filename, true);
    else
        sDownloadSystem->AddToNonBlockingQueue(filename);
}

bool Launcher::IsLastFile(std::string file)
{
    if(sDownloadSystem->LastFile.find(file) != std::string::npos)
        return true;
    return false;
}

void Launcher::mkdir(string dir)
{
    char temp[512];
    std::string cwd = ACE_OS::getcwd (temp, 512);
    cwd.append(dir.c_str());
    #if defined(_WIN32)
        _mkdir(cwd.c_str());
    #else
        ACE_OS::mkdir(cwd.c_str(), 0777);
    #endif
}

void Launcher::Download(string ip, uint16 port, std::string filename)
{
    ACE_Based::Thread BGDLMGR(new BackgroundDownloadMgrRunnable);   
    BGDLMGR.start();
    sBackgroundDownloadSystem->SetConnection(ip, port);
    sBackgroundDownloadSystem->AddToNonBlockingQueue(filename.c_str());
}

uint64 Launcher::GetActualBlock()
{
    return sBackgroundDownloadSystem->GetActualBlock();
}

uint64 Launcher::GetTotalBlocks()
{
    return sBackgroundDownloadSystem->GetTotalBlocks();
}

void Launcher::SetVarCon(string index, string data)
{
    _varMap[index] = data;
}

std::string Launcher::GetStringFromVarCon(string index)
{
    if (!_varMap[index].empty())
        return _varMap[index];
    return "";
}

float Launcher::GetFloatFromVarCon(string index)
{
    if (atof(_varMap[index].c_str()))
        return atof(_varMap[index].c_str());
    return 0;
}

uint32 Launcher::GetUint32FromVarCon(string index)
{
    if (atoi(_varMap[index].c_str()))
        return atoi(_varMap[index].c_str());
    return 0;
}

