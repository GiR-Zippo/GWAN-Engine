#ifndef __CLIENT_SESSION_H
#define __CLIENT_SESSION_H

#include "WorldPacket.hpp"
#include "Socket.hpp"

class ClientSession
{
public:
    static void CreateAndRegister(Socket *socket);

    ClientSession(Socket *socket);
    ~ClientSession();

    ACE_Event_Handler::Reference_Count add_reference();
    ACE_Event_Handler::Reference_Count remove_reference();

    /*********************************************************/
    /***                  I/O HANDLING                     ***/
    /*********************************************************/

    bool Update(uint32 diff);
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
    void Handle_ServerSide(WorldPacket& recvPacket);

    void Handle_CMSG_REQUEST_FILE(WorldPacket &packet);
    void Handle_CMSG_REQUEST_FILEBLOCK(WorldPacket &packet);

    void Handle_CMSG_BGDL_GETFILE(WorldPacket &packet);
private:
    void _handlePacket(WorldPacket &packet);
    void _updateIO();

    Socket *_socket;
    uint32 _sessionFlags;
    ACE_Event_Handler::Reference_Count _referenceCount;
};

#endif /* __CLIENT_SESSION_H */
