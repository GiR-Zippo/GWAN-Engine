#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "ace/OS_NS_sys_time.h"
#include "Define.hpp"

inline uint32 GetMSTime()
{
    static const ACE_Time_Value ApplicationStartTime = ACE_OS::gettimeofday();
    return (ACE_OS::gettimeofday() - ApplicationStartTime).msec();
}

inline uint32 GetMSTimeDiff(uint32 oldMSTime, uint32 newMSTime)
{
    if (oldMSTime > newMSTime)
        return (0xFFFFFFFF - oldMSTime) + newMSTime;
    else
        return newMSTime - oldMSTime;
}

#endif /* __TIMER_H */
