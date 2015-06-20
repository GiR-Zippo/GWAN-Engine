#include "ClientSessionMgr.hpp"

#include "ClientSession.hpp"
#include "Configuration.hpp"
#include "Log.hpp"

ClientSessionMgr::ClientSessionMgr()
{
}

ClientSessionMgr::~ClientSessionMgr()
{
}

void ClientSessionMgr::Open()
{
    int num_threads = sConfiguration->GetIntConfig(SESSION_THREAD_COUNT);
    if (num_threads <= 0)
        num_threads = 1;

    sLog->outString("Max allowed Sessions %d", ACE::max_handles());

    for (int i = 0; i < num_threads; ++i)
    {
        SessionUpdater *updater = new SessionUpdater();
        _updaters.insert(updater);
        updater->Start();
    }
}

void ClientSessionMgr::Close()
{
    for (std::set<SessionUpdater *>::iterator itr = _updaters.begin(); itr != _updaters.end(); ++itr)
    {
        (*itr)->Stop();
        (*itr)->Wait();
        delete (*itr);
    }
}

void ClientSessionMgr::RegisterSession(ClientSession *session)
{
    _getLeastUsedUpdater()->AddSession(session);
}

SessionUpdater* ClientSessionMgr::_getLeastUsedUpdater()
{
    SessionUpdater *updater = NULL;

    for (std::set<SessionUpdater *>::iterator itr = _updaters.begin(); itr != _updaters.end(); ++itr)
    {
        if (!updater)
            updater = *itr;
        else if (updater->Counter() > (*itr)->Counter())
            updater = *itr;
    }

    return updater;
}
