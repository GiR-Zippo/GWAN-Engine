#ifndef __SESSION_UPDATER_HPP
#define __SESSION_UPDATER_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>
#include <set>

#include "LockedQueue.hpp"

class ClientSession;

class SessionUpdater : protected ACE_Task_Base
{
public:
    SessionUpdater();
    ~SessionUpdater();

    int Start();
    void Stop();
    void Wait();

    long Counter();
    int AddSession(ClientSession* sess);

    virtual int svc();

private:
    void _addNewSessions();

    typedef ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> AtomicInt;
    typedef std::set<ClientSession*> SessionSet;
    typedef LockedQueue<ClientSession*, ACE_RW_Mutex> SessionQueue;

    AtomicInt _counter;
    int _threadId;

    SessionQueue _newSessions;
    SessionSet _sessions;

    bool _stop;
};

#endif /* __SESSION_UPDATER_HPP*/
