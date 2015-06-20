#include "Socket.hpp"
#include "ace/os_include/netinet/os_tcp.h"

#include "NetworkMgr.hpp"
#include "FlexiHeader.hpp"

Socket::Socket() : OSSocket(),
    _outBufferSize(65536),
    _outBuffer(0),
    _recvWPct(0),
    _recvPct(),
    _header(sizeof(Flexi::ClientPktHeader)),
    _clientHeaderSize(sizeof(Flexi::ClientPktHeader)),
    _closing(false),
    _reactorInfo(NULL)
{
    msg_queue()->high_water_mark(128*1024*1024);
    msg_queue()->low_water_mark(128*1024*1024);

    reference_counting_policy().value(Reference_Counting_Policy::ENABLED);
}

Socket::~Socket()
{
    WorldPacket* packet = NULL;
    while (_packetQueue.next(packet))
        delete packet;

    if (_recvWPct)
    {
        delete _recvWPct;
        _recvWPct = NULL;
    }

    if (_reactorInfo)
        sNetworkMgr->NotifyRemove(_reactorInfo->ID);
}

bool Socket::IsClosing() const
{
    return _closing || closing_;
}

const std::string& Socket::GetRemoteAddress() const
{
    return _address;
}

int Socket::SendPacket(const WorldPacket &pct)
{
    ACE_GUARD_RETURN(LockType, Guard, _outBufferLock, -1);

    if (IsClosing())
        return -1;

    Flexi::ServerPktHeader header(pct.size() + 2, pct.GetOpcode());
    //_crypt.EncryptSend((uint8 *) header.header, header.getHeaderLength());

    if (_outBuffer->space() >= pct.size() + header.getHeaderLength() && msg_queue()->is_empty())
    {
        // Put the packet on the buffer.
        if (_outBuffer->copy((char *) header.header, header.getHeaderLength()) == -1)
            ACE_ASSERT(false);

        if (!pct.empty())
            if (_outBuffer->copy((char *) pct.contents(), pct.size()) == -1)
                ACE_ASSERT(false);
    }
    else
    {
        // Enqueue the packet.
        ACE_Message_Block *mb;

        ACE_NEW_RETURN(mb, ACE_Message_Block(pct.size() + header.getHeaderLength()), -1);

        mb->copy((char *) header.header, header.getHeaderLength());

        if (!pct.empty())
            mb->copy((const char *) pct.contents(), pct.size());

        if (msg_queue()->enqueue_tail(mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
        {
            sLog->outError("NodeSocket::SendPacket enqueue_tail failed");
            mb->release();
            return -1;
        }
    }

    return _scheduleWakeupOutput();
}

bool Socket::GetNextPacket(WorldPacket* &packet)
{
    if (IsClosing())
        return false;

    return _packetQueue.next(packet);
}

int Socket::open (void *a)
{
    ACE_UNUSED_ARG (a);

    _reactorInfo = sNetworkMgr->GetNextReactor();

    reactor(_reactorInfo->Reactor);

    // Prevent double call to this func.
    if (_outBuffer)
        return -1;

    int SockOutKBuff = sNetworkMgr->GetSockOutKBuff();
    if (SockOutKBuff >= 0)
    {
        if (peer().set_option(SOL_SOCKET, SO_SNDBUF, (void*)&SockOutKBuff, sizeof (int)) == -1 && errno != ENOTSUP)
        {
            sLog->outError("NetworkMgr::SetSocketOptions set_option SO_SNDBUF errno = %s", ACE_OS::strerror(errno));
            return -1;
        }
    }

    if (sNetworkMgr->UseNoDelay())
    {
        const int ndoption = 1;

        if (peer().set_option(ACE_IPPROTO_TCP, TCP_NODELAY, (void*)&ndoption, sizeof (int)) == -1)
        {
            sLog->outError("NetworkMgr::SetSocketOptions: peer().set_option TCP_NODELAY errno = %s", ACE_OS::strerror(errno));
            return -1;
        }
    }

    _outBufferSize = size_t(sNetworkMgr->GetSockOutUBuff());

    // Allocate the buffer.
    ACE_NEW_RETURN (_outBuffer, ACE_Message_Block(_outBufferSize), -1);

    // Store peer address.
    ACE_INET_Addr remote_addr;

    if (peer().get_remote_addr(remote_addr) == -1)
    {
       sLog->outError("Socket::open: peer().get_remote_addr errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    _address = remote_addr.get_host_addr();

    // Register with ACE Reactor
    if (reactor()->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK) == -1)
    {
        sLog->outError("Socket::open: unable to register client handler errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    remove_reference();

    return 0;
}

int Socket::_cancelWakeupOutput()
{
    if (reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        // would be good to store errno from reactor with errno guard
        sLog->outError("Socket::cancel_wakeup_output");
        return -1;
    }

    return 0;
}

int Socket::_scheduleWakeupOutput()
{
    if (reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        sLog->outError("Socket::schedule_wakeup_output");
        return -1;
    }

    return 0;
}

int Socket::_handleOutputQueue(GuardType &g)
{
    if (msg_queue()->is_empty())
        return _cancelWakeupOutput();

    ACE_Message_Block *mblk;

    if (msg_queue()->dequeue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero) == -1)
    {
        sLog->outError("Socket::handle_output_queue dequeue_head");
        return -1;
    }

    const size_t send_len = mblk->length();

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send(mblk->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send(mblk->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
    {
        mblk->release();

        return -1;
    }
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero);
            return _scheduleWakeupOutput();
        }

        mblk->release();
        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        mblk->rd_ptr(static_cast<size_t> (n));

        if (msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero) == -1)
        {
            sLog->outError("Socket::handle_output_queue enqueue_head");
            mblk->release();
            return -1;
        }

        return _scheduleWakeupOutput();
    }
    else //now n == send_len
    {
        mblk->release();

        return msg_queue()->is_empty() ? _cancelWakeupOutput() : ACE_Event_Handler::WRITE_MASK;
    }

    ACE_NOTREACHED(return -1);
}

int Socket::handle_output(ACE_HANDLE)
{
    ACE_GUARD_RETURN(LockType, Guard, _outBufferLock, -1);

    if (IsClosing())
        return -1;

    size_t send_len = _outBuffer->length();

    if (send_len == 0)
        return _handleOutputQueue(Guard);

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send(_outBuffer->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send(_outBuffer->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
        return -1;
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return _scheduleWakeupOutput();

        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        _outBuffer->rd_ptr(static_cast<size_t> (n));

        // move the data to the base of the buffer
        _outBuffer->crunch();

        return _scheduleWakeupOutput();
    }
    else //now n == send_len
    {
        _outBuffer->reset();

        return _handleOutputQueue(Guard);
    }

    ACE_NOTREACHED(return 0);
}

int Socket::_handleInputHeader()
{
    ACE_ASSERT(_recvWPct == NULL);

    ACE_ASSERT(_header.length() == sizeof (Flexi::ClientPktHeader));

    //_crypt.DecryptRecv((uint8 *) _header.rd_ptr(), sizeof (Flexi::ClientPktHeader));

    Flexi::ClientPktHeader &header = *((Flexi::ClientPktHeader *) _header.rd_ptr());

    EndianConvertReverse(header.size);
    EndianConvert(header.cmd);

    if (header.size < 2)
    {
        errno = EINVAL;
        return -1;
    }

    header.size -= 2;

    ACE_NEW_RETURN(_recvWPct, WorldPacket(Opcodes(header.cmd), (uint32) header.size), -1);

    if (header.size > 0)
    {
        _recvWPct->resize((uint32) header.size);
        _recvPct.base((char *) _recvWPct->contents(), _recvWPct->size());
    }
    else
    {
        ACE_ASSERT(_recvPct.space() == 0);
    }

    return 0;
}

int Socket::_processInput(WorldPacket *packet)
{
    _packetQueue.add(packet);
    return 0;
}

int Socket::_handleInputPayload()
{
    // set errno properly here on error !!!
    // now have a header and payload

    ACE_ASSERT(_recvPct.space() == 0);
    ACE_ASSERT(_header.space() == 0);
    ACE_ASSERT(_recvWPct != NULL);

    int result = _processInput(_recvWPct);

    _recvPct.base(NULL, 0);
    _recvPct.reset();
    _recvWPct = NULL;

    _header.reset();

    return result < 0;
}

int Socket::_handleInputMissingData()
{
    char buf [4096];

    ACE_Data_Block db(sizeof(buf),
                        ACE_Message_Block::MB_DATA,
                        buf,
                        0,
                        0,
                        ACE_Message_Block::DONT_DELETE,
                        0);

    ACE_Message_Block message_block(&db, ACE_Message_Block::DONT_DELETE, 0);

    const size_t recv_size = message_block.space();

    const ssize_t n = peer().recv(message_block.wr_ptr(), recv_size);

    if (n <= 0)
        return int(n);

    message_block.wr_ptr (n);

    while (message_block.length() > 0)
    {
        if (_header.space() > 0)
        {
            //need to receive the header
            const size_t to_header = (message_block.length() > _header.space() ? _header.space() : message_block.length());
            _header.copy(message_block.rd_ptr(), to_header);
            message_block.rd_ptr(to_header);

            if (_header.space() > 0)
            {
                // Couldn't receive the whole header this time.
                ACE_ASSERT(message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }

            // We just received nice new header
            if (_handleInputHeader() < 0)
            {
                ACE_ASSERT((errno != EWOULDBLOCK) && (errno != EAGAIN));
                return -1;
            }
        }

        // Its possible on some error situations that this happens
        // for example on closing when epoll receives more chunked data and stuff
        // hope this is not hack, as proper m_RecvWPct is asserted around
        if (!_recvWPct)
        {
            sLog->outError("Forcing close on input m_RecvWPct = NULL");
            errno = EINVAL;
            return -1;
        }

        // We have full read header, now check the data payload
        if (_recvPct.space() > 0)
        {
            //need more data in the payload
            const size_t to_data = (message_block.length() > _recvPct.space() ? _recvPct.space() : message_block.length());
            _recvPct.copy(message_block.rd_ptr(), to_data);
            message_block.rd_ptr(to_data);

            if (_recvPct.space() > 0)
            {
                // Couldn't receive the whole data this time.
                ACE_ASSERT(message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }
        }

        //just received fresh new payload
        if (_handleInputPayload() < 0)
            return -1;
    }

    return size_t(n) == recv_size ? 1 : 2;
}

int Socket::handle_input(ACE_HANDLE)
{
    if (IsClosing())
        return -1;

    switch (_handleInputMissingData())
    {
        case -1 :
        {
            if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
                return IsClosing() ? -1 : 0;

            sLog->outDebug("Socket::handle_input: Peer error closing connection errno = %s", ACE_OS::strerror(errno));

            errno = ECONNRESET;
            return -1;
        }
        case 0:
        {
            sLog->outDebug("Socket::handle_input: Peer has closed connection");

            errno = ECONNRESET;
            return -1;
        }
        case 1:
            return 1;
        default:
            return IsClosing() ? -1 : 0;
    }

    ACE_NOTREACHED(return -1);
}

int Socket::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    if (IsClosing())
        return -1;

    _closing = true;

    // Register with ACE Reactor
    if (_reactorInfo)
    {
        if (reactor()->remove_handler(this, ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL ) == -1)
        {
            sLog->outError("Socket::handle_close: unable to remove client handler errno = %s", ACE_OS::strerror(errno));
            return -1;
        }

        reactor(NULL);
    }

    return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::handle_close(handle, mask);
}
