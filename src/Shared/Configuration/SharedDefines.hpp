#ifndef __SHAREDDEFINES_HPP__
#define __SHAREDDEFINES_HPP__

enum FileReturnCode
{
    FRC_OK       = 0,
    FRC_BEGIN    = 1,
    FRC_END      = 2,
    FRC_NOFILE   = 3,
    FRC_DATA     = 4,
    FRC_BIG_DATA = 5,
    FRC_MAX
};

#endif
