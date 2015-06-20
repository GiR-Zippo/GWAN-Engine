#ifndef __CLIENTSOCKET_HPP
#define __CLIENTSOCKET_HPP

#include "AcceptSocket.hpp"

#include "AcceptSocket.hpp"
#include "SocketAcceptor.hpp"

class ClientSocket : public AcceptSocket
{
public:
    ClientSocket() : AcceptSocket()
    {
    }

private:
    virtual int _processInput(WorldPacket *packet);

    virtual void _buildAuthChallengePacket(WorldPacket &packet);

    int _handleAuthResponse(WorldPacket &packet);
};

#define sClientAcceptor ACE_Singleton<SocketAcceptor<ClientSocket>, ACE_Thread_Mutex>::instance()

#endif /* __CLIENTSOCKET_HPP */
