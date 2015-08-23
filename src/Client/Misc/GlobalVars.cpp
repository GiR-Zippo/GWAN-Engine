#include "GlobalVars.hpp"

void GlobalVars::SetStartupResolution(int x, int y)
{
    _width = x;
    _height = y;
    _currentwidth = x;
    _currentheight = y;
}

void GlobalVars::SetResolution(int x, int y)
{
    _currentwidth = x;
    _currentheight = y;
}