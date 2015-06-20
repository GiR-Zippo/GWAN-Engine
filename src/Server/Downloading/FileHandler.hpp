#ifndef __CLIENT_FILE_HANDLER_H
#define __CLIENT_FILE_HANDLER_H

#include "Common.hpp"
#include <openssl/md5.h>
#include <map>

#define _BLOCKSIZE 5000000

namespace FileHandler
{
    uint64 GetFileSize(std::string filename);
    std::map<uint8, unsigned char> GetMD5(std::string filename);
};

#endif /* __CLIENT_AUTHENTICATION_HANDLER_H */
 
