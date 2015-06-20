#include "Opcodes.hpp"

OpcodeHandler opcodeTable[NUM_OPCODES] =
{
    // These are for the client authentication (not for the player account)
    /*0x0000*/ { "NULL_OPCODE"                      , &ClientSession::Handle_NULL                   },
    /*0x0001*/ { "SMSG_AUTH_CHALLENGE"              , &ClientSession::Handle_ServerSide             },
    /*0x0002*/ { "CMSG_AUTH_RESPONSE"               , &ClientSession::Handle_NULL                   },
    /*0x0003*/ { "SMSG_AUTH_OK"                     , &ClientSession::Handle_ServerSide             },
    /*0x0004*/ { "SMSG_AUTH_FAIL"                   , &ClientSession::Handle_ServerSide             },
    /*0x0005*/ { "CMSG_REQUEST_PACKAGE_INFO"        , &ClientSession::Handle_NULL                   },
    /*0x0006*/ { "SMSG_REQUEST_PACKAGE_RESPONSE"    , &ClientSession::Handle_ServerSide             },
    /*0x0007*/ { "CMSG_REQUEST_FILE"                , &ClientSession::Handle_CMSG_REQUEST_FILE      },
    /*0x0008*/ { "SMSG_SEND_FILE"                   , &ClientSession::Handle_ServerSide             },
    /*0x0009*/ { "SMSG_SEND_FILE_BLOCK"             , &ClientSession::Handle_ServerSide             },
    /*0x000A*/ { "CMSG_REQUEST_FILEBLOCK"           , &ClientSession::Handle_CMSG_REQUEST_FILEBLOCK },
    /*0x000B*/ { "CMSG_BGDL_GETFILE"                , &ClientSession::Handle_CMSG_BGDL_GETFILE      },
    /*0x000C*/ { "SMSG_BGDL_FILEDATA"               , &ClientSession::Handle_ServerSide             }
    
};
