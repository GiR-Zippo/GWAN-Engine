/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __BACKGROUNDDOWNLOADSYSTEM_HPP__
#define __BACKGROUNDDOWNLOADSYSTEM_HPP__

#include <ace/Singleton.h>

#include "Common.h"
#include "Threading.hpp"
#include "LockedQueue.hpp"
#include "BackgroundDownloadSession.hpp"

#include <openssl/md5.h>
#include <map>
#include <list>


class BackgroundDownloadMgrRunnable : public ACE_Based::Runnable
{
    public:
        void run();
};

class BackgroundDownloadSystem
{
    public:
        BackgroundDownloadSystem();
        ~BackgroundDownloadSystem();
        
        void SetConnection(std::string ip, uint16 port) { _session->SetConnection(ip, port); }
        void AddToNonBlockingQueue(std::string filename);
        bool Update(uint32 diff);
        void SetInActive() { _IsActive = false; }
        std::string LastFile;

        uint64 GetActualBlock();
        uint64 GetTotalBlocks();

        bool Terminate;
    private:
        bool _IsActive;
        LockedQueue<FileHeader*, ACE_Thread_Mutex> _BackgroundDownloads;
        LockedQueue<FileHeader*, ACE_Thread_Mutex> _BlockingBackgroundDownloads;

        BackgroundDownloadSession *_session;
};
#define sBackgroundDownloadSystem ACE_Singleton<BackgroundDownloadSystem, ACE_Null_Mutex>::instance()
#endif /*__DOWNLOADSYSTEM_HPP__*/
