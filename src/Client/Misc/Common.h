/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __COMMON_H_
#define __COMMON_H_

#ifdef WIN32
    #define GLEW_STATIC
    #include "GL/glew.h"
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/glew.h>
#endif

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <algorithm>

#include "Define.h"
using namespace std;

#define SERVER_PORT 39235
//#define SERVER_ADDRESS "mail.Laenalith-wow.com"
#define SERVER_ADDRESS "127.0.0.1"

#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

enum SessionFlags
{
    FLAG_SESSION_RECONNECT      = 0x01,
    FLAG_SESSION_WAIT_FOR_OK    = 0x02,
    FLAG_SESSION_CONNECTED      = 0x04,
    FLAG_SESSION_BUSY           = 0x08
};

///- Download
struct FileHeader
{
    std::string name;
    bool hasFile;
    uint64 size;
    std::map<uint8, uint8> hash;
};

//#define _RREL_

static std::string GetPath()
{
    std::string cwd;
#ifdef _RREL_
        char temp[512];
        cwd = ACE_OS::getcwd (temp, 512);
        cwd.append("./Temp/");
#else
        cwd = "/extras/Dev/C++/Lalelu-Launcher/Out/Temp/";
#endif
    return cwd;    
}

static std::string GetFilePath(std::string filename)
{ 
    std::string tempdir;
#ifdef _RREL_
    char temp[512];
    std::string cwd = ACE_OS::getcwd (temp, 512);
    cwd.append("./Temp/");
    #if defined(_WIN32)
        _mkdir(cwd.c_str());
    #else 
        ACE_OS::mkdir(cwd.c_str(), 0777);
    #endif
    tempdir = cwd;
#else
    tempdir = "/extras/Dev/C++/Lalelu-Launcher/Out/Temp/";
#endif
    std::string data = tempdir;
    data.append(filename);
    return data;
}

static std::string GetDownloadFilePath(std::string filename)
{ 
    std::string tempdir;
#ifdef _RREL_
    char temp[512];
    std::string cwd = ACE_OS::getcwd (temp, 512);
    cwd.append("./Download/");
    #if defined(_WIN32)
        _mkdir(cwd.c_str());
    #else 
        ACE_OS::mkdir(cwd.c_str(), 0777);
    #endif
    tempdir = cwd;
#else
    tempdir = "/extras/Dev/C++/Lalelu-Launcher/Out/Download/";
#endif
    std::string data = tempdir;
    data.append(filename);
    return data;
}
#endif
