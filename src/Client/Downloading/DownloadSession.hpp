/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __DOWLOAD_SESSION_H
#define __DOWLOAD_SESSION_H

#include "WorldPacket.hpp"
#include "Socket.hpp"

#define RECONNECT_INTERVAL 1000

class DownloadSession
{
public:
    DownloadSession();
    ~DownloadSession();

    ACE_Event_Handler::Reference_Count add_reference();
    ACE_Event_Handler::Reference_Count remove_reference();

    /*********************************************************/
    /***                  I/O HANDLING                     ***/
    /*********************************************************/

    bool IsConnected() const;

    void Update(uint32 diff);
    void CloseSocket();

    void SendPacket(WorldPacket const &packet);

    /*********************************************************/
    /***                      FLAGS                        ***/
    /*********************************************************/
    void SetFlag(uint32 flag);
    void RemoveFlag(uint32 flag);

    bool HasFlag(uint32 flag) const;
    void ToggleFlag(uint32 flag);

    /*********************************************************/
    /***                  OPCODE HANDLING                  ***/
    /*********************************************************/
    void Handle_NULL(WorldPacket& recvPacket);
    void Handle_ClientSide(WorldPacket& recvPacket);

    void Handle_SMSG_AUTH_CHALLENGE(WorldPacket& recvPacket);
    void Handle_SMSG_AUTH_OK(WorldPacket& recvPacket);
    void Handle_SMSG_AUTH_FAIL(WorldPacket& recvPacket);

    void Handle_SMSG_SEND_FILE(WorldPacket& recvPacket);
    void Handle_SMSG_SEND_FILE_BLOCK(WorldPacket& recvPacket);

private:
    void _handlePacket(WorldPacket &packet);
    void _updateIO();

    Socket *_socket;
    uint32 _reconnectTimer;
    uint32 _sessionFlags;
    ACE_Event_Handler::Reference_Count _referenceCount;

    LockedQueue<WorldPacket*, ACE_RW_Mutex> _packetQueue;
};

#endif /* __DOWLOAD_SESSION_H */
