#ifndef __SERVER_HPP
#define __SERVER_HPP

#include "Define.hpp"

#include <set>

#define _SLEEP_CONST 50

class Server
{
public:
    Server();
    ~Server();

    bool IsRunning() const;
    void Stop();

    void RunUpdateLoop();

private:
    void _update(uint32 diff);
    void _cleanup();

    volatile bool _isRunning;
};

#define sServer ACE_Singleton<Server, ACE_Thread_Mutex>::instance()

#endif /* __SERVER_HPP */
