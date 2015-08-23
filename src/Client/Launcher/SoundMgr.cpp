/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "SoundMgr.hpp"
#include "Sound.hpp"
#include <AL/alure.h>

SoundMgr::SoundMgr()
{
    if(!alureInitDevice(NULL, NULL))
        fprintf(stderr, "Failed to open OpenAL device: %s\n", alureGetErrorString());
    _SoundList.clear();
}

SoundMgr::~SoundMgr()
{}

void SoundMgr::Shutdown()
{
    for (std::map<uint32, Sound*>::const_iterator itr = _SoundList.begin(); itr != _SoundList.end(); itr++)
        delete (*itr).second;
    _SoundList.clear();
    alureShutdownDevice();
}

void SoundMgr::Update()
{
    alureUpdate();
}

uint32 SoundMgr::AddSound(std::string filename)
{
    Sound* sound = new Sound();
    uint32 id = sound->Init(filename);
    _SoundList[id] = sound;
    return id;
}

void SoundMgr::RemoveSound(uint32 soundID)
{
    for (std::map<uint32, Sound*>::iterator itr = _SoundList.begin(); itr != _SoundList.end(); itr++)
    {
        if (soundID == (*itr).first)
        {
            delete (*itr).second;
            _SoundList.erase(itr);
        }
    }
}

Sound* SoundMgr::GetSound(uint32 id)
{
    return _SoundList[id];
}

uint32 SoundMgr::GetSoundIdByName(std::string name)
{
    for (std::map<uint32, Sound*>::const_iterator itr = _SoundList.begin(); itr != _SoundList.end(); itr++)
        if ((*itr).second->GetName().compare(GetFilePath(name)) == 0)
            return (*itr).first;
    return 0;
}
