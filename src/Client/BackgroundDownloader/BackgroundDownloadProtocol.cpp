/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "BackgroundDownloadProtocol.hpp"

BOpcodeHandler bopcodeTable[NUM_OPCODES] =
{
    // These are for the client authentication (not for the player account)
    /*0x0000*/ { "NULL_OPCODE"                      , &BackgroundDownloadSession::Handle_NULL                 },
    /*0x0001*/ { "SMSG_AUTH_CHALLENGE"              , &BackgroundDownloadSession::Handle_SMSG_AUTH_CHALLENGE  },
    /*0x0002*/ { "CMSG_AUTH_RESPONSE"               , &BackgroundDownloadSession::Handle_ClientSide           },
    /*0x0003*/ { "SMSG_AUTH_OK"                     , &BackgroundDownloadSession::Handle_SMSG_AUTH_OK         },
    /*0x0004*/ { "SMSG_AUTH_FAIL"                   , &BackgroundDownloadSession::Handle_SMSG_AUTH_FAIL       },
    /*0x0005*/ { "CMSG_REQUEST_PACKAGE_INFO"        , &BackgroundDownloadSession::Handle_NULL                 },
    /*0x0006*/ { "SMSG_REQUEST_PACKAGE_RESPONSE"    , &BackgroundDownloadSession::Handle_NULL                 },
    /*0x0007*/ { "CMSG_REQUEST_FILE"                , &BackgroundDownloadSession::Handle_NULL                 },
    /*0x0008*/ { "SMSG_SEND_FILE"                   , &BackgroundDownloadSession::Handle_NULL                 },
    /*0x0009*/ { "SMSG_SEND_FILE_BLOCK"             , &BackgroundDownloadSession::Handle_NULL                 },
    /*0x000A*/ { "CMSG_REQUEST_FILEBLOCK"           , &BackgroundDownloadSession::Handle_NULL                 },
    /*0x000B*/ { "CMSG_BGDL_GETFILE"                , &BackgroundDownloadSession::Handle_ClientSide           },
    /*0x000B*/ { "SMSG_BGDL_FILEDATA"               , &BackgroundDownloadSession::Handle_SMSG_BGDL_FILEDATA   }
};
