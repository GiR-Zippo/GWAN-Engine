#ifndef __NETWORKMGR_HPP
#define __NETWORKMGR_HPP

#include <ace/Singleton.h>
#include <ace/Reactor.h>
#include <ace/Atomic_Op.h>

#include <map>

class Socket;

struct ReactorInfo
{
    ACE_Reactor * Reactor;
    int ThreadGroup;
    int Count;
    int ID;
};

class NetworkMgr
{
public:
    friend class ACE_Singleton<NetworkMgr, ACE_Thread_Mutex>;

    int Open();
    void Close();
    void Wait();

    int GetCompressionLevel() const;
    int GetSockOutKBuff() const;
    int GetSockOutUBuff() const;
    bool UseNoDelay() const;

    ACE_Reactor * Reactor();
    ReactorInfo * GetNextReactor();
    void NotifyRemove(int ID);

private:
    NetworkMgr();
    ~NetworkMgr();

    ACE_Reactor * _createReactor();

    size_t _threadsCount;
    int _threadGroup;

    ACE_Reactor * _reactor;

    std::map<int, ReactorInfo *> _reactors;

    bool _useNoDelay;
    int _sockOutKBuff;
    int _sockOutUBuff;
    int _compressionLevel;
};

#define sNetworkMgr ACE_Singleton<NetworkMgr, ACE_Thread_Mutex>::instance()

#endif /* __NETWORKMGR_HPP */
