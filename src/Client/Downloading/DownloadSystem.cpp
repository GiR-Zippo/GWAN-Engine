/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "DownloadSystem.hpp"
#include "OpcodeDefines.hpp"
#include "WorldPacket.hpp"
#include "Timer.hpp"
#include "FileHelper.hpp"

#include <ace/OS.h>

#define _SLEEP_CONST 50

void DownloadMgrRunnable::run()
{
    uint32 realCurrTime = 0;
    uint32 realPrevTime = GetMSTime();

    uint32 prevSleepTime = 0;                               // used for balanced full tick time length near WORLD_SLEEP_CONST

    ///- While we have not World::m_stopEvent, update the world
    while (!sDownloadSystem->Terminate)
    {
        realCurrTime = GetMSTime();
        uint32 diff = GetMSTimeDiff(realPrevTime,realCurrTime);
        sDownloadSystem->Update(diff);
        realPrevTime = realCurrTime;
        // diff (D0) include time of previous sleep (d0) + tick time (t0)
        // we want that next d1 + t1 == WORLD_SLEEP_CONST
        // we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
        // d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
        if (diff <= _SLEEP_CONST+prevSleepTime)
        {
            prevSleepTime = _SLEEP_CONST+prevSleepTime-diff;
            ACE_Based::Thread::Sleep(prevSleepTime);
        }
        else
            prevSleepTime = 0;
    }
}

DownloadSystem::DownloadSystem()
{
    Terminate = false;
    _IsLocked = false;
    _IsActive = false;
}

DownloadSystem::~DownloadSystem()
{
    Terminate = true;
}

void DownloadSystem::AddToNonBlockingQueue(std::string filename)
{
    FileHeader *file     = new FileHeader;
    std::string fullpath = GetFilePath(filename);
    uint64 filesize      = GetFileSize(fullpath);

    file->name           = filename;
    file->hash           = GetMD5(fullpath);
    file->size           = filesize;
    if (filesize == 0)
        file->hasFile = false;
    else
        file->hasFile = true;
    _Downloads.add(file);
}

void DownloadSystem::AddToBlockingQueue(std::string filename, bool force)
{
    _IsActive = true;
    _IsLocked = true;
    std::string fullpath                = GetFilePath(filename);
    uint64 filesize                     = GetFileSize(fullpath);
    std::map<uint8, unsigned char> hash = GetMD5(fullpath);

    //So mal das File beschaffen
    WorldPacket packet(CMSG_REQUEST_FILE);
    packet << filename;

    //Haben wir es schon?
    if (force || (filesize == 0))
        packet << uint8(0);
    else
        packet << uint8(1);

    //Werfen wir den Hash in den Raum
    if (!hash.empty())
    {
        packet << uint8(1); //Hat ne Checksum
        for (uint8 i = 0; i < MD5_DIGEST_LENGTH; i++)
            packet << uint8(hash[i]);
    }
    else
        packet << uint8(0); //Hat keine Checksum

    packet << filesize;     //Und auch gleich die Größe nechwerfen
    _session.SendPacket(packet);

    while(_IsLocked)
    {
        #ifdef WIN32
            Sleep(10);
        #else
            usleep(10000);   // usleep takes sleep time in us
        #endif
    }
}

bool DownloadSystem::Update(uint32 diff)
{
    _session.Update(diff);

    if (_session.HasFlag(FLAG_SESSION_CONNECTED))
    {
        FileHeader *file;
        if (!_Downloads.empty())
        {
            _IsActive = true;
            _Downloads.next(file);

            //So mal das File beschaffen
            WorldPacket packet(CMSG_REQUEST_FILE);
            packet << file->name;
            //Haben wir es schon?
            if (file->hasFile)
                packet << uint8(1);
            else
                packet << uint8(0);

            //Werfen wir den Hash in den Raum
            if (!file->hash.empty())
            {
                packet << uint8(1); //Hat ne Checksum
                for (uint8 i = 0; i < MD5_DIGEST_LENGTH; i++)
                    packet << uint8(file->hash[i]);
            }
            else
                packet << uint8(0); //Hat keine Checksum

            packet << file->size; //Und auch gleich die Größe nechwerfen
            _session.SendPacket(packet);
        }
    }
    return true;
}