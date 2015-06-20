#include "ClientSocket.hpp"
#include "ClientSession.hpp"

bool SecureCompare(const char *a, size_t aSize, const char *b, size_t bSize)
{
    size_t diff = aSize ^ bSize;

    for (size_t indx = 0; indx < aSize && indx < bSize; ++indx)
        diff |= a[indx] ^ b[indx];

    return diff == 0;
}

void ClientSocket::_buildAuthChallengePacket(WorldPacket &packet)
{
    packet.Initialize(SMSG_AUTH_CHALLENGE);
    packet << "WHO ARE YOU?";
}

int ClientSocket::_handleAuthResponse(WorldPacket &packet)
{
    std::string answer;
    packet >> answer;

    if (!SecureCompare(answer.c_str(), answer.length(), "DARTH VADER!", strlen("DARTH VADER!")))
    {
        sLog->outString("ClientSocket::_handleAuthResponse: authentication fail!");

        WorldPacket data(SMSG_AUTH_FAIL, 0);
        SendPacket(data);
        return -1;
    }

    sLog->outString("ClientSocket::_handleAuthResponse: authentication success!");
    ClientSession::CreateAndRegister(this);

    SetAuthenticated();

    WorldPacket data(SMSG_AUTH_OK, 0);
    SendPacket(data);
    return 0;
}
