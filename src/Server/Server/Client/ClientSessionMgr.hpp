#ifndef __CLIENT_SESSION_MGR_HPP
#define __CLIENT_SESSION_MGR_HPP

#include "SessionUpdater.hpp"

#include <set>

class ClientSessionMgr
{
public:
    ClientSessionMgr();
    ~ClientSessionMgr();

    void Open();
    void Close();

    void RegisterSession(ClientSession *session);

private:
    SessionUpdater* _getLeastUsedUpdater();

    std::set<SessionUpdater *> _updaters;
};

#define sClientSessionMgr ACE_Singleton<ClientSessionMgr, ACE_Thread_Mutex>::instance()

#endif /* __CLIENT_SESSION_MGR_HPP */
