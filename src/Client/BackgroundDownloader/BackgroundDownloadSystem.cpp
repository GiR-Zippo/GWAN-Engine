/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "BackgroundDownloadSystem.hpp"
#include "OpcodeDefines.hpp"
#include "WorldPacket.hpp"
#include "Timer.hpp"
#include "FileHelper.hpp"

#include <ace/OS.h>
#define _SLEEP_CONST 50

void BackgroundDownloadMgrRunnable::run()
{
    uint32 realCurrTime = 0;
    uint32 realPrevTime = GetMSTime();

    uint32 prevSleepTime = 0;

    ///- While we have not World::m_stopEvent, update the world
    while (!sBackgroundDownloadSystem->Terminate)
    {
        realCurrTime = GetMSTime();
        uint32 diff = GetMSTimeDiff(realPrevTime,realCurrTime);
        sBackgroundDownloadSystem->Update(diff);
        realPrevTime = realCurrTime;
        if (diff <= _SLEEP_CONST+prevSleepTime)
        {
            prevSleepTime = _SLEEP_CONST+prevSleepTime-diff;
            ACE_Based::Thread::Sleep(prevSleepTime);
        }
        else
            prevSleepTime = 0;
    }
}

BackgroundDownloadSystem::BackgroundDownloadSystem()
{
    _session = new BackgroundDownloadSession();
    Terminate = false;
    _IsActive = false;
}

BackgroundDownloadSystem::~BackgroundDownloadSystem()
{
    Terminate = true;
    delete _session;
}

void BackgroundDownloadSystem::AddToNonBlockingQueue(std::string filename)
{
    FileHeader *file     = new FileHeader;
    std::string fullpath = GetDownloadFilePath(filename);
    uint64 filesize      = GetFileSize(fullpath);

    file->name           = filename;
    file->hash           = GetMD5(fullpath);
    file->size           = filesize;
    if (filesize == 0)
        file->hasFile = false;
    else
        file->hasFile = true;
    _BackgroundDownloads.add(file);
}

bool BackgroundDownloadSystem::Update(uint32 diff)
{
    _session->Update(diff);

    if (_session->HasFlag(FLAG_SESSION_CONNECTED))
    {
        FileHeader *file;
        if (!_session->HasFlag(FLAG_SESSION_BUSY))
        {
            if (!_BackgroundDownloads.empty())
            {
                _IsActive = true;
                _BackgroundDownloads.next(file);

                //So mal das File beschaffen
                WorldPacket packet(CMSG_BGDL_GETFILE);
                packet << uint64(0);  //Wir wollen den Anfang
                packet << file->name; //Von dem File
                _session->SendPacket(packet);

                //Und jetzt ist man beschäftigt
                _session->SetFlag(FLAG_SESSION_BUSY);
            }
        }
    }
    return true;
}

uint64 BackgroundDownloadSystem::GetActualBlock()
{
    if (_session)
        return _session->GetEtaBlocks();
    return 0;
}

uint64 BackgroundDownloadSystem::GetTotalBlocks()
{
    if (_session)
        return _session->GetTotalBlocks();
    return 0;
}
