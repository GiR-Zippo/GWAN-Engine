#ifndef __P_MISC__
#define __P_MISC__

#include <string.h>

inline bool strContains(const std::string inputStr, const std::string searchStr)
{
        size_t contains;

        contains = inputStr.find(searchStr);

        if(contains != std::string::npos)
                return true;
        else
                return false;
}

inline std::string ConvertPath(std::string dataPath)
{
    // for empty string use current dir as for absent case
    if (dataPath.empty())
        dataPath = "./";
    // normalize dir path to path/ or path\ form
    else if (dataPath.at(dataPath.length() - 1) != '/' && dataPath.at(dataPath.length() - 1) != '\\')
        dataPath.append("/");
    return dataPath;
}

#endif
