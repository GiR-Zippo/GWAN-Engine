#include "Server.hpp"

#include <ace/OS.h>
#include "Timer.hpp"

Server::Server() : _isRunning(true)
{
}

Server::~Server()
{
}

bool Server::IsRunning() const
{
    return _isRunning;
}

void Server::Stop()
{
    _isRunning = false;
}

void Server::_update(uint32 diff)
{
}

void Server::_cleanup()
{
}

void Server::RunUpdateLoop()
{
    uint32 realCurrTime = 0;
    uint32 realPrevTime = GetMSTime();

    uint32 prevSleepTime = 0; // used for balanced full tick time length

    while (_isRunning)
    {
        realCurrTime = GetMSTime();
        uint32 diff = GetMSTimeDiff(realPrevTime, realCurrTime);
        _update(diff);
        realPrevTime = realCurrTime;
        // diff (D0) include time of previous sleep (d0) + tick time (t0)
        // we want that next d1 + t1 == _SLEEP_CONST
        // we can't know next t1 and then can use (t0 + d1) == _SLEEP_CONST requirement
        // d1 = _SLEEP_CONST - t0 = _SLEEP_CONST - (D0 - d0) = _SLEEP_CONST + d0 - D0
        if (diff <= _SLEEP_CONST + prevSleepTime)
        {
            prevSleepTime = _SLEEP_CONST + prevSleepTime - diff;
            ACE_OS::sleep(ACE_Time_Value(0, static_cast<uint64>(prevSleepTime) * 1000L));
        }
        else
            prevSleepTime = 0;
    }

    _cleanup();
}
