/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __DOWNLOAD_PROTOCOL_HPP
#define __DOWNLOAD_PROTOCOL_HPP

#include "BackgroundDownloadSession.hpp"

struct BOpcodeHandler
{
    char const* name;
    void (BackgroundDownloadSession::*handler)(WorldPacket& recvPacket);
};

extern BOpcodeHandler bopcodeTable[NUM_OPCODES];

#endif /* __DOWNLOAD_PROTOCOL_HPP */
