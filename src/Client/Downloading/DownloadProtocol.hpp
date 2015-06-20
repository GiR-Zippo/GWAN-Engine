/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __DOWNLOAD_PROTOCOL_HPP
#define __DOWNLOAD_PROTOCOL_HPP

#include "DownloadSession.hpp"

struct OpcodeHandler
{
    char const* name;
    void (DownloadSession::*handler)(WorldPacket& recvPacket);
};

extern OpcodeHandler opcodeTable[NUM_OPCODES];

#endif /* __DOWNLOAD_PROTOCOL_HPP */
