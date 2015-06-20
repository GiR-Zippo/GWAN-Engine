/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "DownloadSession.hpp"

#include "Common.h"
#include "DownloadProtocol.hpp"
#include "SocketConnector.hpp"

DownloadSession::DownloadSession() : _referenceCount(1), _sessionFlags(0), _socket(NULL), _reconnectTimer(RECONNECT_INTERVAL)
{
    SetFlag(FLAG_SESSION_RECONNECT); //Die Verbindung baut nun der Reconnect auf
}

DownloadSession::~DownloadSession()
{
    RemoveFlag(FLAG_SESSION_RECONNECT);

    if (_socket)
    {
        _socket->close();
        _socket->remove_reference();
        _socket = NULL;
    }
}

ACE_Event_Handler::Reference_Count DownloadSession::add_reference()
{
    return ++_referenceCount;
}

ACE_Event_Handler::Reference_Count DownloadSession::remove_reference()
{
    ACE_Event_Handler::Reference_Count result = --_referenceCount;

    if (result == 0)
        delete this;

    return result;
}

/*********************************************************/
/***                  I/O HANDLING                     ***/
/*********************************************************/
bool DownloadSession::IsConnected() const
{
    if (_socket)
        return !_socket->IsClosing();

    return false;
}

void DownloadSession::_handlePacket(WorldPacket &packet)
{
    try
    {
        (this->*opcodeTable[packet.GetOpcode()].handler)(packet);
    }
    catch (ByteBufferException &)
    {
        sLog->outDebug("Dumping error causing packet:");
    }
}

void DownloadSession::_updateIO()
{
    WorldPacket* packet = NULL;

    while (_socket && HasFlag(FLAG_SESSION_CONNECTED) && _packetQueue.next(packet))
    {
        _socket->SendPacket(*packet);

        delete packet;
    }

    while (_socket && _socket->GetNextPacket(packet))
    {
        if (packet->GetOpcode() < NUM_OPCODES)
            _handlePacket(*packet);

        delete packet;
    }
}

void DownloadSession::Update(uint32 diff)
{
    if (HasFlag(FLAG_SESSION_RECONNECT))
    {
        if (_reconnectTimer < diff)
        {
            _reconnectTimer += RECONNECT_INTERVAL;

            if (Socket *socket = sSocketConnector->OpenConnection(SERVER_PORT, SERVER_ADDRESS))
            {
                std::cout << "OPENED CONNECTION\n";
                socket->add_reference();
                _socket = socket;

                RemoveFlag(FLAG_SESSION_RECONNECT);
                SetFlag(FLAG_SESSION_WAIT_FOR_OK);
            }
        }

        _reconnectTimer -= diff;
    }

    _updateIO();

    if (_socket && _socket->IsClosing())
    {
        std::cout << "DISCONNECTED\n";
        _socket->remove_reference();
        _socket = NULL;

        SetFlag(FLAG_SESSION_RECONNECT);
        RemoveFlag(FLAG_SESSION_CONNECTED);
    }
}

void DownloadSession::CloseSocket()
{
    if (_socket)
        _socket->close();
}

void DownloadSession::SendPacket(WorldPacket const &packet)
{
    if (!_socket)
    {
        _packetQueue.add(new WorldPacket(packet));
        return;
    }

    if (_socket->SendPacket(packet) == -1)
        CloseSocket();
}

/*********************************************************/
/***                      FLAGS                        ***/
/*********************************************************/
void DownloadSession::SetFlag(uint32 flag)
{
    _sessionFlags |= flag;
}

void DownloadSession::RemoveFlag(uint32 flag)
{
    _sessionFlags &= ~flag;
}

void DownloadSession::ToggleFlag(uint32 flag)
{
    if (HasFlag(flag))
        RemoveFlag(flag);
    else
        SetFlag(flag);
}

bool DownloadSession::HasFlag(uint32 flag) const
{
    return (_sessionFlags & flag) != 0;
}

/*********************************************************/
/***                  OPCODE HANDLING                  ***/
/*********************************************************/
void DownloadSession::Handle_NULL(WorldPacket& recvPacket)
{
}

void DownloadSession::Handle_ClientSide(WorldPacket& recvPacket)
{
}

void DownloadSession::Handle_SMSG_AUTH_CHALLENGE(WorldPacket& recvPacket)
{
    WorldPacket packet(CMSG_AUTH_RESPONSE);

    packet << "DARTH VADER!";
    SendPacket(packet);
}

void DownloadSession::Handle_SMSG_AUTH_OK(WorldPacket& recvPacket)
{
    if (HasFlag(FLAG_SESSION_WAIT_FOR_OK))
    {
        std::cout << "Handle_SMSG_AUTH_OK\n";
        SetFlag(FLAG_SESSION_CONNECTED);
        RemoveFlag(FLAG_SESSION_WAIT_FOR_OK);
    }
}

void DownloadSession::Handle_SMSG_AUTH_FAIL(WorldPacket& recvPacket)
{
    std::cout << "Handle_SMSG_AUTH_FAIL\n";
    RemoveFlag(FLAG_SESSION_WAIT_FOR_OK);
    CloseSocket();
}
