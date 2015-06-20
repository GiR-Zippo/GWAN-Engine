/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __FILEHELPER_HPP__
#define __FILEHELPER_HPP__

#include <map>
#include <ace/OS.h>
#include "openssl/md5.h"
#include "Common.h"

uint64 GetFileSize(std::string filename);
std::map<uint8, uint8> GetMD5(std::string filename);

#endif
