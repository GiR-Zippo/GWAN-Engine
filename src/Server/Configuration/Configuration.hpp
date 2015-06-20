#ifndef __P_CONFIGURATION__
#define __P_CONFIGURATION__

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <string>

enum StringConfig
{
    CONFIG_PATH = 0,
    CACHE_FILES,
    MAX_STRING_CONFIG
};

enum BoolConfig
{
    USE_CACHE = 0,
    LOG_ENABLED,
    MAX_BOOL_CONFIG
};

enum IntConfig
{
    CACHE_TIME = 0,
    LISTEN_PORT,
    SESSION_THREAD_COUNT,
    MAX_INT_CONFIG
};

class Configuration
{
public:
    void Load();

    std::string GetStringConfig(StringConfig index) const
    {
        return index < MAX_STRING_CONFIG ? stringConfiguration[index]  : 0;
    }

    bool GetBoolConfig(BoolConfig index) const
    {
        return index < MAX_BOOL_CONFIG ? boolConfiguration[index] : 0;
    }

    int GetIntConfig(IntConfig index) const
    {
        return index < MAX_INT_CONFIG ? intConfiguration[index] : 0;
    }

private:
    std::string stringConfiguration[MAX_STRING_CONFIG];
    bool boolConfiguration[MAX_BOOL_CONFIG];
    int intConfiguration[MAX_INT_CONFIG];
};

#define sConfiguration ACE_Singleton<Configuration, ACE_Null_Mutex>::instance()

#endif 
