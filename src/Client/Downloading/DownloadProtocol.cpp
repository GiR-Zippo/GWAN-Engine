/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "DownloadProtocol.hpp"

OpcodeHandler opcodeTable[NUM_OPCODES] =
{
    // These are for the client authentication (not for the player account)
    /*0x0000*/ { "NULL_OPCODE"                      , &DownloadSession::Handle_NULL                 },
    /*0x0001*/ { "SMSG_AUTH_CHALLENGE"              , &DownloadSession::Handle_SMSG_AUTH_CHALLENGE  },
    /*0x0002*/ { "CMSG_AUTH_RESPONSE"               , &DownloadSession::Handle_ClientSide           },
    /*0x0003*/ { "SMSG_AUTH_OK"                     , &DownloadSession::Handle_SMSG_AUTH_OK         },
    /*0x0004*/ { "SMSG_AUTH_FAIL"                   , &DownloadSession::Handle_SMSG_AUTH_FAIL       },
    /*0x0005*/ { "CMSG_REQUEST_PACKAGE_INFO"        , &DownloadSession::Handle_ClientSide           },
    /*0x0006*/ { "SMSG_REQUEST_PACKAGE_RESPONSE"    , &DownloadSession::Handle_NULL                 },
    /*0x0007*/ { "CMSG_REQUEST_FILE"                , &DownloadSession::Handle_ClientSide           },
    /*0x0008*/ { "SMSG_SEND_FILE"                   , &DownloadSession::Handle_SMSG_SEND_FILE       },
    /*0x0009*/ { "SMSG_SEND_FILE_BLOCK"             , &DownloadSession::Handle_SMSG_SEND_FILE_BLOCK },
    /*0x000A*/ { "CMSG_REQUEST_FILEBLOCK"           , &DownloadSession::Handle_ClientSide           },
    /*0x000B*/ { "CMSG_BGDL_GETFILE"                , &DownloadSession::Handle_ClientSide           },
    /*0x000C*/ { "SMSG_BGDL_FILEDATA"               , &DownloadSession::Handle_NULL                 }
};
