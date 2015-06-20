#include "SessionUpdater.hpp"
#include "ClientSession.hpp"
#include "Timer.hpp"

#define SESSION_SLEEP_CONST 50 //milliseconds

SessionUpdater::SessionUpdater() : _counter(0), _threadId(-1), _stop(false)
{
}

SessionUpdater::~SessionUpdater()
{
    Stop();
    Wait();
}

int SessionUpdater::Start()
{
    if (_threadId != -1)
        return -1;

    return (_threadId = activate());
}

void SessionUpdater::Stop()
{
    _stop = true;
}

void SessionUpdater::Wait()
{
    ACE_Task_Base::wait();
}

long SessionUpdater::Counter()
{
    return static_cast<long> (_counter.value());
}

int SessionUpdater::AddSession(ClientSession* session)
{
    ++_counter;
    session->add_reference();
    _newSessions.add(session);
    return 0;
}

void SessionUpdater::_addNewSessions()
{
    ClientSession *session = NULL;

    while (_newSessions.next(session))
        _sessions.insert(session);
}

int SessionUpdater::svc()
{
    uint32 realCurrTime = 0;
    uint32 realPrevTime = GetMSTime();

    uint32 prevSleepTime = 0;

    while (!_stop)
    {
        realCurrTime = GetMSTime();
        uint32 diff = GetMSTimeDiff(realPrevTime, realCurrTime);

        _addNewSessions();

        if (_sessions.empty())
            ACE_OS::sleep(ACE_Time_Value(0, 500000));

        for (SessionSet::iterator itr = _sessions.begin(); itr != _sessions.end();)
        {
            if (!(*itr))
            {
                _sessions.erase(itr++);
                --_counter;
            }
            else if (!(*itr)->Update(diff))
            {
                (*itr)->remove_reference();
                _sessions.erase(itr++);
                --_counter;
            }
            else
                ++itr;
        }

        realPrevTime = realCurrTime;
        if (diff <= SESSION_SLEEP_CONST + prevSleepTime)
        {
            prevSleepTime = SESSION_SLEEP_CONST + prevSleepTime - diff;
            ACE_OS::sleep(ACE_Time_Value(0, prevSleepTime * 1000));
        }
        else
            prevSleepTime = 1;

    }
    return 0;
}
