#include "ClientSession.hpp"
#include "ClientSessionMgr.hpp"
#include "Opcodes.hpp"

void ClientSession::CreateAndRegister(Socket *socket)
{
    ClientSession *session = new ClientSession(socket);

    sClientSessionMgr->RegisterSession(session);

    session->remove_reference();
}

ClientSession::ClientSession(Socket *socket) : _referenceCount(1), _sessionFlags(0), _socket(socket)
{
    _socket->add_reference();
}

ClientSession::~ClientSession()
{
    if (_socket)
    {
        _socket->close();
        _socket->remove_reference();
        _socket = NULL;
    }
}

ACE_Event_Handler::Reference_Count ClientSession::add_reference()
{
    return ++_referenceCount;
}

ACE_Event_Handler::Reference_Count ClientSession::remove_reference()
{
    ACE_Event_Handler::Reference_Count result = --_referenceCount;

    if (result == 0)
        delete this;

    return result;
}

/*********************************************************/
/***                  I/O HANDLING                     ***/
/*********************************************************/
void ClientSession::_handlePacket(WorldPacket &packet)
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

void ClientSession::_updateIO()
{
    WorldPacket* packet = NULL;

    while (_socket && _socket->GetNextPacket(packet))
    {
        if (packet->GetOpcode() < NUM_OPCODES)
            _handlePacket(*packet);

        delete packet;
    }
}

bool ClientSession::Update(uint32 diff)
{
    _updateIO();

    if (_socket && _socket->IsClosing())
    {
        _socket->remove_reference();
        _socket = NULL;
    }

    return _socket != NULL;
}

void ClientSession::CloseSocket()
{
    if (_socket)
        _socket->close();
}

void ClientSession::SendPacket(WorldPacket const &packet)
{
    if (!_socket)
        return;

    if (_socket->SendPacket(packet) == -1)
        CloseSocket();
}

/*********************************************************/
/***                      FLAGS                        ***/
/*********************************************************/
void ClientSession::SetFlag(uint32 flag)
{
    _sessionFlags |= flag;
}

void ClientSession::RemoveFlag(uint32 flag)
{
    _sessionFlags &= ~flag;
}

void ClientSession::ToggleFlag(uint32 flag)
{
    if (HasFlag(flag))
        RemoveFlag(flag);
    else
        SetFlag(flag);
}

bool ClientSession::HasFlag(uint32 flag) const
{
    return (_sessionFlags & flag) != 0;
}

/*********************************************************/
/***                  OPCODE HANDLING                  ***/
/*********************************************************/
void ClientSession::Handle_NULL(WorldPacket& recvPacket)
{
}

void ClientSession::Handle_ServerSide(WorldPacket& recvPacket)
{
}
