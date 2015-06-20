/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Sound.hpp"

Sound::Sound()
{
    _playing = false;
}

Sound::~Sound()
{
    if (_playing)
        StopSound();

    alDeleteSources(1, &_src);
    alDeleteBuffers(1, &_buf);
}

void Sound::PlaySound()
{
    if (_playing)
        StopSound();

    alSourcei(_src, AL_BUFFER, _buf);

    if(alurePlaySource(_src, NULL, NULL) == AL_FALSE)
    {
        fprintf(stderr, "Failed to start source!\n");
        alDeleteSources(1, &_src);
        alDeleteBuffers(1, &_buf);
        return;
    }
    alSourcef(_src, AL_GAIN, 0.05f);
    _playing = true;
}

void Sound::StopSound()
{
    if (_playing)
        alureStopSource(_src, false);
    _playing = false;
}

uint32 Sound::Init(std::string filename)
{
    _filename = filename;
    alGenSources(1, &_src);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Failed to create OpenAL source!\n");
        alureShutdownDevice();
    }
    
    _buf = alureCreateBufferFromFile(filename.c_str());
    if(!_buf)
    {
        fprintf(stderr, "Could not load %s: %s\n", filename.c_str(), alureGetErrorString());
        alDeleteSources(1, &_src);

        alureShutdownDevice();
    }

    return _src;
}

void Sound::SetVolume(float vol)
{
    alSourcef(_src, AL_GAIN, vol);
}
