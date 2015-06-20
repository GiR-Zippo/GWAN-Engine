#ifndef __SOCKETCONNECTOR_HPP__
#define __SOCKETCONNECTOR_HPP__

#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>

#include "NetworkMgr.hpp"
#include "Socket.hpp"

class SocketConnector
{
public:
    SocketConnector()
    {
        m_Connector = new ACE_Connector<Socket, ACE_SOCK_Connector>;
    }

    ~SocketConnector()
    {
        delete m_Connector;
    }

    int Open()
    {
        return m_Connector->open(sNetworkMgr->Reactor(), ACE_NONBLOCK);
    }

    int Close()
    {
        return m_Connector->close();
    }

    virtual Socket * OpenConnection(uint16 port, std::string IP)
    {
        Socket *sock = NULL;
        ACE_INET_Addr port_to_connect(port, IP.c_str());

        if (m_Connector->connect(sock, port_to_connect) < 0)
        {
            sock->remove_reference();
            return NULL;
        }

        return sock;
    }

private:
    ACE_Connector<Socket, ACE_SOCK_Connector> *m_Connector;
};

#define sSocketConnector ACE_Singleton<SocketConnector, ACE_Thread_Mutex>::instance()

#endif /* __SOCKETCONNECTOR_HPP__ */
