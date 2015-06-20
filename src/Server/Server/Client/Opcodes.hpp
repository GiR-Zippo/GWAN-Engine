#ifndef __OPCODES_HPP
#define __OPCODES_HPP

#include "ClientSession.hpp"

struct OpcodeHandler
{
    char const* name;
    void (ClientSession::*handler)(WorldPacket& recvPacket);
};

extern OpcodeHandler opcodeTable[NUM_OPCODES];

#endif /* __OPCODES_HPP */
