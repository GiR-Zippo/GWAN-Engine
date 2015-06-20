/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __SOUNDMGR_HPP__
#define __SOUNDMGR_HPP__

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include "Common.h"
#include <map>

class Sound;
class SoundMgr
{
    public:
        SoundMgr();
        ~SoundMgr();
        void Shutdown();
        void Update();
        
        uint32 AddSound(std::string filename);
        void RemoveSound(uint32 soundID);
        Sound* GetSound(uint32 id);
        uint32 GetSoundIdByName(std::string name);
    private:
        std::map<uint32, Sound*> _SoundList;
};
#define sSoundMgr ACE_Singleton<SoundMgr, ACE_Null_Mutex>::instance()
#endif
