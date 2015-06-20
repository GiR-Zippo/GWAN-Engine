#include "Config.hpp"
#include "Configuration.hpp"
#include "Log.hpp"

void Configuration::Load()
{
    std::string dataPath = ConfigMgr::GetStringDefault("DataDir", "");
    // for empty string use current dir as for absent case
    if (dataPath.empty())
        dataPath = "./";
    // normalize dir path to path/ or path\ form
    else if (dataPath.at(dataPath.length() - 1) != '/' && dataPath.at(dataPath.length() - 1) != '\\')
        dataPath.append("/");

    stringConfiguration[CONFIG_PATH] = dataPath;

    intConfiguration[LISTEN_PORT] = ConfigMgr::GetIntDefault("ListenPort", 3000);
    intConfiguration[SESSION_THREAD_COUNT] = ConfigMgr::GetIntDefault("SessionHandling.Threads", 1);

    stringConfiguration[CACHE_FILES] = ConfigMgr::GetStringDefault("Cache.Files", "");
    boolConfiguration[USE_CACHE] = ConfigMgr::GetBoolDefault("Cache.Enabled", false);
    intConfiguration[CACHE_TIME] = ConfigMgr::GetIntDefault("Cache.Time", 60);
}
