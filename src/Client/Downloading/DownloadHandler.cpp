/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "DownloadSession.hpp"
#include "DownloadSystem.hpp"
#include "SharedDefines.hpp"
#include "FileHelper.hpp"

void DownloadSession::Handle_SMSG_SEND_FILE(WorldPacket& recvPacket)
{
#if WIN32
    _fmode = _O_BINARY;
#endif

    std::string filename;
    uint8 fStatus;
    uint64 size;

    recvPacket >> filename;
    recvPacket >> fStatus;

    if (fStatus == FRC_OK || fStatus == FRC_NOFILE)
    {
        sDownloadSystem->LastFile = filename;
        sDownloadSystem->Unlock();
        return;
    }
    else if (fStatus == FRC_DATA || fStatus == FRC_BIG_DATA)
    {
        recvPacket >> size;
        std::ofstream myfile;
        myfile.open(GetFilePath(filename).c_str(), std::ios::out | std::ios::binary);
        for (uint64 in = 0; in != size; in++)
        {
            unsigned char d;
            recvPacket >> d;
            myfile << d;
        }
        myfile.close();

        if (fStatus == FRC_BIG_DATA)
        {
            WorldPacket response(CMSG_REQUEST_FILEBLOCK);
            response << filename;
            response << GetFileSize(GetFilePath(filename).c_str());
            SendPacket(response);
        }
    }
}

void DownloadSession::Handle_SMSG_SEND_FILE_BLOCK(WorldPacket& recvPacket)
{
#if WIN32
    _fmode = _O_BINARY;
#endif

    std::string filename;
    uint64 size;

    //Filename
    recvPacket >> filename;

    //Größe
    uint64 fsize = GetFileSize(GetFilePath(filename));

    recvPacket >> size;
    std::ofstream myfile;
    myfile.open(GetFilePath(filename).c_str(), ios::out | ios::app | std::ios::binary);
    myfile.seekp(fsize);
    for (uint64 in = 0; in != size; in++)
    {
        unsigned char d;
        recvPacket >> d;
        myfile << d;
    }
    myfile.close();
    WorldPacket response(CMSG_REQUEST_FILEBLOCK);
    response << filename;
    response << GetFileSize(GetFilePath(filename).c_str());
    SendPacket(response);
    return;
}
