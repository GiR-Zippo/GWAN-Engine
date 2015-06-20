/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __SOUND_H__
#define __SOUND_H__

#include "Common.h"

#ifdef WIN32
#include <al.h>
#include <alc.h>
#include <AL/alure.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alure.h>
#endif

class Sound
{
    public:
        Sound();
        ~Sound();

        uint32 Init(std::string filename);
        void PlaySound();
        void StopSound();
        std::string GetName() { return _filename; }
        void SetVolume(float vol);
    private:
        bool        _playing;
        ALuint      _src;
        ALuint      _buf;
        std::string _filename;
};

#endif
