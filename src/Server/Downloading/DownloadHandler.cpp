#include "ClientSession.hpp"
#include "Configuration.hpp"
#include "FileHandler.hpp"
#include "SharedDefines.hpp"
#include "ace/OS.h"
#include "zlib.h"

void ClientSession::Handle_CMSG_REQUEST_FILE(WorldPacket& packet)
{
    sLog->outString("Handle_CMSG_REQUEST_FILE");

    std::string dataPath = sConfiguration->GetStringConfig(CONFIG_PATH);
    bool sameCheckSum = true;                  //Ja ist true ich weiﬂ, gleich aber nicht mehr
    std::string File;                             //Dateiname
    uint8 hasChecksum;                            //Hat das File aufm Client ne Checksum
    uint8 hasFileOnClient;                        //Haben wir das File dort?
    std::map<uint8, unsigned char> clientFileHash;//Hashkeks
    size_t ClientFileSize = 0;                    //DateiGrˆﬂe
    size_t ServerFileSize = 0;

    packet >> File;                               //Lass mal Filenamen holen
    dataPath.append(File);                        //Daraus bauen wir mal den kompletten Pfad
    std::cout << dataPath << "\n";
    packet >> hasFileOnClient;                    //Gibts das File beim Client

    if (hasFileOnClient == 1)
    {
        packet >> hasChecksum;                    //Hat es Hash

        if (hasChecksum == 1)
        {
            //Her damit und vergleichen
            for (uint8 i = 0; i < MD5_DIGEST_LENGTH; i++)
            {
                uint8 bt;
                packet >> bt;
                clientFileHash[i] = bt;
                printf("%X02", bt);
            }

            std::cout << "\n";

            std::map<uint8, unsigned char> hash = FileHandler::GetMD5(dataPath.c_str());

            for (uint8 i = 0; i < MD5_DIGEST_LENGTH; i++)
            {
                printf("%X02", hash[i]);

                if ((uint8) clientFileHash[i] != (uint8) hash[i])
                    sameCheckSum = false;         //Scheiﬂ Hash, hab besseres hier
            }

            std::cout << "\n";
        }

        //So noch schnell Grˆﬂe vergleichen
        packet >> ClientFileSize;
        ServerFileSize = FileHandler::GetFileSize(dataPath);

        if (sameCheckSum && (ClientFileSize == ServerFileSize))
        {
            WorldPacket data(SMSG_SEND_FILE, 1 + File.length() + 4);
            data << File;
            data << uint8(FRC_OK);
            SendPacket(data);
            return;
        }
    }

    ACE_HANDLE hfile = ACE_OS::open(dataPath.c_str(), GENERIC_READ | FILE_FLAG_OVERLAPPED);

    if ((int) hfile == -1)
    {
        WorldPacket data(SMSG_SEND_FILE, 1 + File.length() + 4);
        data << File;
        data << uint8(FRC_NOFILE);
        SendPacket(data);
        return;
    }
    else
    {
        ServerFileSize = FileHandler::GetFileSize(dataPath);
        uint64 size = 0;
        uint8* buffer;

        std::ifstream sourcestr;
        sourcestr.open(dataPath.c_str(), std::ios::in | std::ios::binary);
        std::filebuf* pbuf = sourcestr.rdbuf();

        if (ServerFileSize <= _BLOCKSIZE)
            size = ServerFileSize;
        else
            size = _BLOCKSIZE;

        buffer = new uint8[size + 1];
        pbuf->sgetn((char*) buffer, size);

        WorldPacket data(SMSG_SEND_FILE, size + 1 + File.length() + 4);
        data << File;

        if (ServerFileSize <= _BLOCKSIZE)
            data << uint8(FRC_DATA);
        else
            data << uint8(FRC_BIG_DATA);

        data << size;
        data.append(buffer, size);
        SendPacket(data);
        sourcestr.close();

        if (ServerFileSize <= _BLOCKSIZE)
        {
            WorldPacket data(SMSG_SEND_FILE, 1 + File.length() + 4);
            data << File;
            data << uint8(FRC_OK);
            SendPacket(data);
        }
    }
}

void ClientSession::Handle_CMSG_REQUEST_FILEBLOCK(WorldPacket& packet)
{
    sLog->outString("Handle_CMSG_REQUEST_FILEBLOCK");

    std::string File;
    int len;
    size_t ServerFileSize = 0;
    packet >> File;
    packet >> len;

    std::string dataPath = sConfiguration->GetStringConfig(CONFIG_PATH);
    dataPath.append(File);

    ServerFileSize = FileHandler::GetFileSize(dataPath);
    uint64 size = 0;
    uint8* buffer;

    std::ifstream sourcestr;
    sourcestr.open(dataPath.c_str() , std::ios::in | std::ios::binary);
    std::filebuf* pbuf = sourcestr.rdbuf();
    pbuf->pubseekpos (len, std::ios::in | std::ios::binary);

    if ((ServerFileSize - len) <= _BLOCKSIZE)
        size = (ServerFileSize - len);
    else
        size = _BLOCKSIZE;

    //Ineffizient aber geht
    if (size == 0)
    {
        WorldPacket data(SMSG_SEND_FILE, 1 + File.length() + 4);
        data << File;
        data << uint8(FRC_OK);
        SendPacket(data);
        sourcestr.close();
        return;
    }

    buffer = new uint8[size + 1];
    pbuf->sgetn ((char*)buffer, size + 1);

    WorldPacket data(SMSG_SEND_FILE_BLOCK, size + 1 + File.length() + 4);
    data << File;
    data << size;
    data.append(buffer, size);
    SendPacket(data);
    sourcestr.close();
}

std::string string_compress(std::string const& s)
{
    z_streamp *zc = new z_streamp();
    unsigned int sourceSize = s.size();
    const char* source = s.c_str();

    unsigned long dsize = sourceSize + (sourceSize * 0.1f) + 16;
    char* destination = new char[dsize];

    int result = compress((unsigned char*)destination, &dsize, (const unsigned char*)source, sourceSize);

    if (result != Z_OK)
        std::cout << "Compress error occured! Error code: " << result << std::endl;

    delete zc;
    std::string(destination, dsize);
    return std::string(destination, dsize);
}

void ClientSession::Handle_CMSG_BGDL_GETFILE(WorldPacket& packet)
{
    sLog->outString("Handle_CMSG_BGDL_GETFILE");
    uint32 blocksize = 5000000;
    uint64 readoffset;
    std::string filename;
    packet >> readoffset;
    packet >> filename;

    std::string file = sConfiguration->GetStringConfig(CONFIG_PATH);
    file.append(filename);

    ACE_OFF_T ServerFileSize = FileHandler::GetFileSize(file);
    uint64 ServerBlocks = ServerFileSize / blocksize;
    uint64 ClientBlocks = readoffset / blocksize;
    std::cout << ClientBlocks << ":" << ServerBlocks << "\n";
    std::ifstream ifile;
    ifile.open(file.c_str(), std::ios::in | std::ios::binary);
    ifile.seekg(readoffset);

    bool multiblock = false;
    uint64 counter = 0;
    std::stringstream ins;

    while (true)
    {
        char t = ifile.get();

        if (ifile.eof())
            break;

        if (counter > blocksize)
        {
            multiblock = true;
            WorldPacket data(SMSG_BGDL_FILEDATA);
            data << filename;
            data << ClientBlocks;
            data << ServerBlocks;

            if (readoffset == 0)
                data << uint8(FRC_BEGIN);
            else
                data << uint8(FRC_DATA);

            data << uint32(ins.str().length());
            data.append(ins.str().c_str(), ins.str().length());

            SendPacket(data);

            ins.str(std::string());
            ifile.close();
            return;
        }

        ins << t;
        counter++;
    }

    WorldPacket data(SMSG_BGDL_FILEDATA);
    data << filename;
    data << ClientBlocks;
    data << ServerBlocks;

    if (readoffset == 0 && multiblock == false)
        data << uint8(FRC_OK);
    else
        data << uint8(FRC_END);

    data << uint32(ins.str().length());
    data.append(ins.str().c_str(), ins.str().length());

    SendPacket(data);
    ifile.close();
}
