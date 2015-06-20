/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Common.h"
#include "ScriptObject.hpp"
#include "SoundMgr.hpp"
#include "Sound.hpp"

uint32 ScriptObject::LoadSound(string file)
{
    return sSoundMgr->AddSound(GetFilePath(file));
}

void ScriptObject::PlaySound(uint32 id)
{
    Sound* sound = sSoundMgr->GetSound(id);
    if (sound)
        sound->PlaySound();
}

void ScriptObject::StopSound(uint32 id)
{
    Sound* sound = sSoundMgr->GetSound(id);
    if (sound)
        sound->StopSound();
}

void ScriptObject::SetSoundVolume(uint32 id, float vol)
{
    Sound* sound = sSoundMgr->GetSound(id);
    if (sound)
        sound->SetVolume(vol);
}

uint32 ScriptObject::GetSoundIdByName(string name)
{
    return sSoundMgr->GetSoundIdByName(name);
}

void ScriptObject::UnloadSound(uint32 id)
{
    sSoundMgr->RemoveSound(id);
} 
