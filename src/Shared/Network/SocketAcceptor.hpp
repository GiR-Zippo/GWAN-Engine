#ifndef __SOCKETACCEPTOR_HPP__
#define __SOCKETACCEPTOR_HPP__

#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>

#include "Acceptor_BASE.hpp"
#include "NetworkMgr.hpp"

class ACE_Event_Handler;

template <class SocketType>
class SocketAcceptor
{
public:
    friend ACE_Singleton<SocketAcceptor<SocketType>, ACE_Thread_Mutex>;

    int Open(ACE_UINT16 port, const char* address)
    {
        _acceptor = new Acceptor_BASE<SocketType>;

        if (_acceptor->open(ACE_INET_Addr(port, address), sNetworkMgr->Reactor(), ACE_NONBLOCK) == -1)
        {
            sLog->outError("Failed to open acceptor, check if the port is free");
            return -1;
        }
        return 0;
    }

    void Close()
    {
        if (_acceptor)
            _acceptor->close();
    }

private:
    SocketAcceptor() : _acceptor(NULL)
    {
    }

    ~SocketAcceptor()
    {
        delete _acceptor;
    }

    Acceptor_BASE<SocketType> *_acceptor;
};

#endif /* __SOCKETACCEPTOR_HPP__ */
