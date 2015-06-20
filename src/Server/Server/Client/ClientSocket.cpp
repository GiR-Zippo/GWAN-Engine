#include "ClientSocket.hpp"

int ClientSocket::_processInput(WorldPacket *packet)
{
    switch (packet->GetOpcode())
    {
        case CMSG_AUTH_RESPONSE:
            if (IsAuthenticated())
                return -1;

            return _handleAuthResponse(*packet);
        default:
            if (!IsAuthenticated())
                return -1;

            return Socket::_processInput(packet);
    }
}
