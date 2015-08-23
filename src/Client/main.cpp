/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Common.h"
#include "GlobalVars.hpp"
#include "wrapper.h"
#include "Launcher.hpp"
#include "NetworkMgr.hpp"
#include "SocketConnector.hpp"
#include "SoundMgr.hpp"
#include "DownloadSystem.hpp"
#include "ObjectMgr.hpp"

#include "Shader.hpp"

ACE_Based::Thread DLMGR;
ACE_Based::Thread OMGR;
bool terminating;

Wrapper *wrapper;

void display()
{
    wrapper->Draw();
}

void idle()
{
    wrapper->Idle();
}

void timer(int = 0)
{
    uint8 t = 1;//50;
    wrapper->Tick(t);
    glutTimerFunc(t, timer, 0);
}

void mouse(int button, int state, int x, int y)
{
    wrapper->Mouse(button, state, x,y);
}

void keyboardSpecial(int key, int xmouse, int ymouse)
{
    wrapper->KeyBoardSpecial(key, xmouse, ymouse);
}

void keyboardNormal(unsigned char key, int xmouse, int ymouse)
{
    wrapper->KeyBoardNormal(key, xmouse, ymouse);
}

void resize (int w, int h)
{
    wrapper->Resize(w, h);
}

void motion(int x, int y)
{
    wrapper->Motion(x, y);
}

void windowclosed()
{
    if (!terminating)
    {
        terminating = true;
        sObjectMgr->Terminate();
        OMGR.destroy();
        sDownloadSystem->Terminate = true;
        DLMGR.destroy();
        sSoundMgr->Shutdown();
        sSocketConnector->Close();
        sNetworkMgr->Close();
    }
}

int main(int argc, char **argv)
{
    terminating = false;
    ACE_OS::sleep(ACE_Time_Value(0, 1000 * 100));
    //Erstmal das DownloadTeil anwerfen
    ACE_Based::Thread DLMGR(new DownloadMgrRunnable);
    DLMGR.start();
    ACE_Based::Thread OMGR(new ObjectMgrRunnable);
    OMGR.start();
    sNetworkMgr->Open();
    sSocketConnector->Open();

    wrapper = new Launcher();
    wrapper->Init();
    sGlobalVars->SetStartupResolution(1280, 768);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(sGlobalVars->GetStartupWidth(), sGlobalVars->GetStartupHeight());
    glutCreateWindow("GWAN");

    /* Versions und EngineName */
    cout << "GWAN-Launcher\n";
    cout << "Engine: Natalia v0.7.2\n";
    cout << "(c) 2013-2015 GiR-Zippo@hellokitty.com\n";
    /* Check GLEW */
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    const GLubyte *str;

    /*Check openGL */
    str= glGetString(GL_VENDOR);
    printf("Graphic-Vendor: %s\n", str);
    str = glGetString(GL_RENDERER);
    printf("Graphic-Renderer: %s\n", str);
    str = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", str);
    if ((str[0] << 16) < 2) 
    {
        printf("OpenGL 2 not supported!\n");
        exit(-1);
    }
    str = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GLSL Version: %s\n", str);
    str = glewGetString(GLEW_VERSION);
    printf("Glew Version: %s\n", str);

#ifndef SHOW_CONSOLE
    #ifdef WIN32
        FreeConsole();
    #endif
#endif

    //Weiter gehts
    wrapper->InitGL();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIdleFunc (idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);
    glutKeyboardFunc(keyboardNormal);
    glutSpecialFunc(keyboardSpecial);
    timer();
    glutCloseFunc(windowclosed);
    glutMainLoop();

    return 0;
}
