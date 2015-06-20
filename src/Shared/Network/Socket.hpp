#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <ace/Basic_Types.h>
#include <ace/Synch_Traits.h>
#include <ace/SOCK_Stream.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Message_Block.h>
#include <ace/Svc_Handler.h>
#include <ace/Unbounded_Queue.h>

#include "Log.hpp"
#include "WorldPacket.hpp"
#include "LockedQueue.hpp"

struct ReactorInfo;

typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> OSSocket;

class Socket : public OSSocket
{
public:
    typedef ACE_Thread_Mutex LockType;
    typedef ACE_Guard<LockType> GuardType;

    Socket();
    virtual ~Socket();

    bool IsClosing() const;
    const std::string& GetRemoteAddress() const;

    int SendPacket(const WorldPacket &pct);

    bool GetNextPacket(WorldPacket* &packet);

    // Zeug der internen Verwaltung
    virtual int open(void *a);
    virtual int handle_output(ACE_HANDLE = ACE_INVALID_HANDLE);
    virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE,
        ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

protected:
    virtual int _processInput(WorldPacket *packet);

private:
    int _cancelWakeupOutput();
    int _scheduleWakeupOutput();
    int _handleOutputQueue(GuardType &g);

    int _handleInputHeader();
    int _handleInputPayload();
    int _handleInputMissingData();

    bool _closing;
    std::string _address;

    ReactorInfo *_reactorInfo;

    const uint32 _clientHeaderSize;
    ACE_Message_Block _header;
    ACE_Message_Block _recvPct;
    WorldPacket *_recvWPct;
    LockedQueue<WorldPacket*, ACE_RW_Mutex> _packetQueue;

    LockType _outBufferLock;
    size_t _outBufferSize;
    ACE_Message_Block *_outBuffer;
};

#endif /* __SOCKET_H_ */
