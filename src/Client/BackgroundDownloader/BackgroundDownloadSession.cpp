/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "BackgroundDownloadSession.hpp"

#include "Common.h"
#include "BackgroundDownloadProtocol.hpp"
#include "SocketConnector.hpp"

BackgroundDownloadSession::BackgroundDownloadSession() : _referenceCount(1), _sessionFlags(0), _socket(NULL), _reconnectTimer(RECONNECT_INTERVAL)
{
    _address.clear();
    _port = 0;
    _etaBlocks = 0;
    _totalBlocks = 0;
    SetFlag(FLAG_SESSION_RECONNECT); //Die Verbindung baut nun der Reconnect auf
}

BackgroundDownloadSession::~BackgroundDownloadSession()
{
    RemoveFlag(FLAG_SESSION_RECONNECT);

    if (_socket)
    {
        _socket->close();
        _socket->remove_reference();
        _socket = NULL;
    }
}

ACE_Event_Handler::Reference_Count BackgroundDownloadSession::add_reference()
{
    return ++_referenceCount;
}

ACE_Event_Handler::Reference_Count BackgroundDownloadSession::remove_reference()
{
    ACE_Event_Handler::Reference_Count result = --_referenceCount;

    if (result == 0)
        delete this;

    return result;
}

/*********************************************************/
/***                  I/O HANDLING                     ***/
/*********************************************************/
bool BackgroundDownloadSession::IsConnected() const
{
    if (_socket)
        return !_socket->IsClosing();

    return false;
}

void BackgroundDownloadSession::_handlePacket(WorldPacket &packet)
{
    try
    {
        (this->*bopcodeTable[packet.GetOpcode()].handler)(packet);
    }
    catch (ByteBufferException &)
    {
        sLog->outDebug("Dumping error causing packet:");
    }
}

void BackgroundDownloadSession::_updateIO()
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

void BackgroundDownloadSession::Update(uint32 diff)
{
    if (HasFlag(FLAG_SESSION_RECONNECT))
    {
        if (_reconnectTimer < diff)
        {
            _reconnectTimer += RECONNECT_INTERVAL;

            if (_port == 0 || _address.empty())
                return;
            if (Socket *socket = sSocketConnector->OpenConnection(_port, _address.c_str()))
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

void BackgroundDownloadSession::CloseSocket()
{
    if (_socket)
        _socket->close();
}

void BackgroundDownloadSession::SendPacket(WorldPacket const &packet)
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
void BackgroundDownloadSession::SetFlag(uint32 flag)
{
    _sessionFlags |= flag;
}

void BackgroundDownloadSession::RemoveFlag(uint32 flag)
{
    _sessionFlags &= ~flag;
}

void BackgroundDownloadSession::ToggleFlag(uint32 flag)
{
    if (HasFlag(flag))
        RemoveFlag(flag);
    else
        SetFlag(flag);
}

bool BackgroundDownloadSession::HasFlag(uint32 flag) const
{
    return (_sessionFlags & flag) != 0;
}

/*********************************************************/
/***                  OPCODE HANDLING                  ***/
/*********************************************************/
void BackgroundDownloadSession::Handle_NULL(WorldPacket& recvPacket)
{
}

void BackgroundDownloadSession::Handle_ClientSide(WorldPacket& recvPacket)
{
}

void BackgroundDownloadSession::Handle_SMSG_AUTH_CHALLENGE(WorldPacket& recvPacket)
{
    WorldPacket packet(CMSG_AUTH_RESPONSE);

    packet << "DARTH VADER!";
    SendPacket(packet);
}

void BackgroundDownloadSession::Handle_SMSG_AUTH_OK(WorldPacket& recvPacket)
{
    if (HasFlag(FLAG_SESSION_WAIT_FOR_OK))
    {
        std::cout << "Handle_SMSG_AUTH_OK\n";
        SetFlag(FLAG_SESSION_CONNECTED);
        RemoveFlag(FLAG_SESSION_WAIT_FOR_OK);
    }
}

void BackgroundDownloadSession::Handle_SMSG_AUTH_FAIL(WorldPacket& recvPacket)
{
    std::cout << "Handle_SMSG_AUTH_FAIL\n";
    RemoveFlag(FLAG_SESSION_WAIT_FOR_OK);
    CloseSocket();
}
