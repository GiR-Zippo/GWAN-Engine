#ifndef _OPCODEDEFINES_H
#define _OPCODEDEFINES_H

enum Opcodes
{
    NULL_OPCODE                         = 0x0000,
    // These are for the client authentication (not for the player account)
    SMSG_AUTH_CHALLENGE                 = 0x0001,
    CMSG_AUTH_RESPONSE                  = 0x0002,
    SMSG_AUTH_OK                        = 0x0003,
    SMSG_AUTH_FAIL                      = 0x0004,

    CMSG_REQUEST_PACKAGE_INFO           = 0x0005,
    SMSG_REQUEST_PACKAGE_RESPONSE       = 0x0006,

    CMSG_REQUEST_FILE                   = 0x0007, ///- string FileName, uint8 forceDL, uint8 hasChecksum, uint[x] checksum or 0, uint64 filesize
    SMSG_SEND_FILE                      = 0x0008, ///- 
    SMSG_SEND_FILE_BLOCK                = 0x0009, ///-
    CMSG_REQUEST_FILEBLOCK              = 0x000A, ///-

    ///- Background Downloader
    CMSG_BGDL_GETFILE                   = 0x000B, ///- uint64 Offset, std::string Filename
    SMSG_BGDL_FILEDATA                  = 0x000C, ///- std::string Filename, uint64 cBlocks, uint64 sBlocks, uint8 State, uint32 Container-length, uint8[CLength]
    NUM_OPCODES
};

#endif
