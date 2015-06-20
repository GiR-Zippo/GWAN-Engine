#include "Config.hpp"
#include "Log.hpp"


#include <stdarg.h>
#include <stdio.h>

Log::Log()
{
    _enabled = true;
}

void Log::Initialize()
{
    _debug   = ConfigMgr::GetIntDefault("Log.Debug.On", true);
    _enabled = ConfigMgr::GetBoolDefault("Log.Enabled", false);
}

void Log::outString(const char * str, ...)
{
    if (!_enabled)
        return;
    
    if (!str)
        return;

  va_list args;
  va_start (args, str);
  vprintf (str, args);
  va_end (args);
  printf("\n");
}

void Log::outString()
{
    if (!_enabled)
        return;
    
    printf("\n");
}

void Log::outError( )
{   
    printf("\n");
}

void Log::outError(const char * str, ...)
{   
    if (!str)
        return;

  va_list args;
  va_start (args, str);
  vprintf (str, args);
  va_end (args);
  printf("\n");
}

void Log::outDebug(const char * str, ...)
{
    if (!_debug)
        return;

    if (!str)
        return;

  va_list args;
  va_start (args, str);
  vprintf (str, args);
  va_end (args);
  printf("\n");
}