/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __DOWNLOADSYSTEM_HPP__
#define __DOWNLOADSYSTEM_HPP__

#include <ace/Singleton.h>

#include "Common.h"
#include "Threading.hpp"
#include "LockedQueue.hpp"
#include "DownloadSession.hpp"

#include <openssl/md5.h>
#include <map>
#include <list>

class DownloadMgrRunnable : public ACE_Based::Runnable
{
    public:
        void run();
};

class DownloadSystem
{
    public:
        DownloadSystem();
        ~DownloadSystem();
        void AddToNonBlockingQueue(std::string filename);
        void AddToBlockingQueue(std::string filename, bool force = false);
        bool Update(uint32 diff);
        void Unlock()
        {
            _IsLocked = false;
            _IsActive = false;
        }
        std::string LastFile;

        bool Terminate;
    private:
        bool _IsLocked;
        bool _IsActive;
        LockedQueue<FileHeader*, ACE_Thread_Mutex> _Downloads;
        LockedQueue<FileHeader*, ACE_Thread_Mutex> _BlockingDownloads;

        DownloadSession _session;
};
#define sDownloadSystem ACE_Singleton<DownloadSystem, ACE_Null_Mutex>::instance()
#endif /*__DOWNLOADSYSTEM_HPP__*/
