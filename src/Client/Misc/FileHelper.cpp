/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "FileHelper.hpp"


uint64 GetFileSize(std::string filename)
{
    uint64 size = 0;
#if WIN32
    _fmode = _O_BINARY;
#endif

    ACE_HANDLE hfile = ACE_OS::open (filename.c_str(), GENERIC_READ | FILE_FLAG_OVERLAPPED);
    if ((int) hfile == -1)
        size = 0;
    else
    {
        size = ACE_OS::filesize(hfile);
        ACE_OS::close(hfile);
    }
    return size;
}


std::map<uint8, uint8> GetMD5(std::string filename)
{
    std::map<uint8, uint8> outHash;
    outHash.clear();

    uint8 c[MD5_DIGEST_LENGTH];
    MD5_CTX mdContext;
    int bytes = -2;
    unsigned char data[1024];

#if WIN32
    _fmode = _O_BINARY;
#endif

    ACE_HANDLE hfile = ACE_OS::open (filename.c_str(), O_RDONLY, GENERIC_READ | FILE_FLAG_OVERLAPPED);
    if ((int)hfile == -1)
        return outHash;

    MD5_Init (&mdContext);
    while (true)
    {
        bytes =  ACE_OS::read(hfile, data, 1024);
        if (bytes == -1 ||  bytes == 0)
            break;
        MD5_Update (&mdContext, data, bytes);
    }
    MD5_Final (c,&mdContext);
    ACE_OS::close(hfile);

    for(uint8 i = 0; i < MD5_DIGEST_LENGTH; i++)
        outHash[i] = c[i];
    return outHash;
}

