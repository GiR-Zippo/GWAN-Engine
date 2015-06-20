#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <stdio.h>
#include <ace/Singleton.h>

class Log
{
    public:
        Log();
        void Initialize();
        void outString(const char * str, ...); //                 ATTR_PRINTF(2, 3);
        void outString();
        void outError();
        void outError(const char * str, ...); //                 ATTR_PRINTF(2, 3);
        void outDebug(const char * str, ...); //                 ATTR_PRINTF(2, 3);

    private:
        bool _enabled;
        bool _debug;
};
#define sLog ACE_Singleton<Log, ACE_Thread_Mutex>::instance()
#endif
