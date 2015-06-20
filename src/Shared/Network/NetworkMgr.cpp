#include "NetworkMgr.hpp"

#include <ace/TP_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/Basic_Types.h>
#include <ace/os_include/netinet/os_tcp.h>

#include "Socket.hpp"
#include "Config.hpp"
#include "Log.hpp"

ACE_THR_FUNC_RETURN event_loop(void *arg)
{
    ACE_Reactor *reactor = (ACE_Reactor *) arg;

    while (!reactor->reactor_event_loop_done())
    {
        ACE_Time_Value interval(0, 10000);

        if (reactor->handle_events(interval) < 0)
            return 0;
    }

   return 0;
}

int NetworkMgr::Open()
{
    _compressionLevel = ConfigMgr::GetIntDefault("Compression", 1);

    _useNoDelay = ConfigMgr::GetBoolDefault("Network.TcpNodelay", true);

    // -1 means use default
    _sockOutKBuff = ConfigMgr::GetIntDefault ("Network.OutKBuff", -1);
    _sockOutUBuff = ConfigMgr::GetIntDefault ("Network.OutUBuff", 65536);
    if (_sockOutUBuff <= 0)
    {
        sLog->outError("Network.OutUBuff is wrong in your config file");
        return -1;
    }

    _threadsCount = ConfigMgr::GetIntDefault("Network.Threads", 1);
    if (_threadsCount <= 0)
    {
        sLog->outError("Network.Threads is wrong in your config file");
        return -1;
    }

    _reactor = _createReactor();
    _threadGroup = ACE_Thread_Manager::instance()->spawn_n(1, event_loop, _reactor);

    for (size_t i = 0; i < _threadsCount; ++i)
    {
        ReactorInfo *info = new ReactorInfo();
        info->Reactor = _createReactor();
        info->ThreadGroup = ACE_Thread_Manager::instance()->spawn_n(1, event_loop, info->Reactor);
        info->Count = 0;
        info->ID = i;

        _reactors[i] = info;
    }

    sLog->outString("Max allowed connections %d", ACE::max_handles());
    return 0;
}

void NetworkMgr::Close()
{
    _reactor->end_reactor_event_loop();

    Wait();
}

void NetworkMgr::Wait()
{
    ACE_Thread_Manager::instance()->wait_grp(_threadGroup);
}

int NetworkMgr::GetCompressionLevel() const
{
    return this->_compressionLevel;
}

int NetworkMgr::GetSockOutKBuff() const
{
    return _sockOutKBuff;
}

int NetworkMgr::GetSockOutUBuff() const
{
    return _sockOutUBuff;
}

bool NetworkMgr::UseNoDelay() const
{
    return _useNoDelay;
}

ACE_Reactor * NetworkMgr::Reactor()
{
    return _reactor;
}

NetworkMgr::NetworkMgr() :
    _threadsCount(0),
    _threadGroup(-1),
    _reactor(NULL),
    _sockOutKBuff(-1),
    _sockOutUBuff(65536),
    _useNoDelay(true),
    _compressionLevel(0)
{
}

NetworkMgr::~NetworkMgr()
{
    delete _reactor;
}

ACE_Reactor * NetworkMgr::_createReactor()
{
    ACE_Reactor_Impl* imp = 0;

#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)

    imp = new ACE_Dev_Poll_Reactor();

    imp->max_notify_iterations (128);
    imp->restart (1);

#else

    imp = new ACE_TP_Reactor();
    imp->max_notify_iterations(128);

#endif

    return new ACE_Reactor(imp, 1);
}

ReactorInfo * NetworkMgr::GetNextReactor()
{
    std::map<int, ReactorInfo *>::iterator itr = _reactors.begin();
    int ID = (*itr).first;
    ReactorInfo* info = (*itr).second;

    for (; itr != _reactors.end(); ++itr)
    {
        ReactorInfo* tmp = (*itr).second;

        if (tmp->Count < info->Count)
        {
            info = tmp;
            ID = (*itr).first;
        }
    }
    return info;
}

void NetworkMgr::NotifyRemove(int ID)
{
    if (_reactor)
        _reactors[ID]->Count--;    
}
