#ifndef __ACCEPTSOCKET_HPP
#define __ACCEPTSOCKET_HPP

#include "Socket.h"

class AcceptSocket : public Socket
{
public:
    AcceptSocket() : Socket(), _authenticated(false)
    {
    }

    bool IsAuthenticated() const
    {
        return _authenticated;
    }

    virtual int open(void *a)
    {
        if (Socket::open(a) < 0)
            return -1;

        WorldPacket packet;
        _buildAuthChallengePacket(packet);
        return this->SendPacket(packet);
    }

protected:
    void SetAuthenticated()
    {
        _authenticated = true;
    }

private:
    bool _authenticated;

    virtual void _buildAuthChallengePacket(WorldPacket &packet) = 0;
};

#endif /* __ACCEPTSOCKET_HPP */
