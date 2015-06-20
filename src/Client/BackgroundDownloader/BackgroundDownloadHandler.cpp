/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "BackgroundDownloadSession.hpp"
#include "BackgroundDownloadSystem.hpp"
#include "SharedDefines.hpp"
#include "zlib.h"

std::string string_uncompress(std::string const& s, unsigned long len)
{
    unsigned int sourceSize = s.size();
    const char* source = s.c_str();

    char* destination = new char[len];

    int result = uncompress((unsigned char*)destination, &len, (const unsigned char*)source, sourceSize);

    if (result != Z_OK)
        std::cout << "Uncompress error occured! Error code: " << result << std::endl;

    return destination;
}

void BackgroundDownloadSession::Handle_SMSG_BGDL_FILEDATA(WorldPacket& recvPacket)
{
#if WIN32
    _fmode = _O_BINARY;
#endif
    std::string filename;
    uint8 fStatus;
    uint64 cBlocks;
    uint64 sBlocks;

    recvPacket >> filename;
    recvPacket >> cBlocks;
    recvPacket >> sBlocks;
    recvPacket >> fStatus;
    _etaBlocks = cBlocks;
    _totalBlocks = sBlocks;
    std::ofstream myfile;

    if (fStatus == FRC_BEGIN || fStatus == FRC_OK)
        myfile.open(GetDownloadFilePath(filename).c_str(), std::ios::out | std::ios::binary);
    else if (fStatus == FRC_DATA || fStatus == FRC_END)
    {
        myfile.open(GetDownloadFilePath(filename).c_str(), std::ios::app | std::ios::binary);
        RemoveFlag(FLAG_SESSION_BUSY);
    }

    uint32 length;
    recvPacket >> length;
    std::string data;

    for (uint32 i = 0; i != length; i++)
    {
        uint8 d;
        recvPacket >> d;
        data += ((const char)d);
    }
    myfile << data;

    if (fStatus == FRC_BEGIN || fStatus == FRC_DATA)
    {
        WorldPacket data(CMSG_BGDL_GETFILE);
        data << uint64(myfile.tellp());//Wir wollen den Anfang
        data << filename; //Von dem File
        SendPacket(data);
    }

    myfile.close();
}