/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "ObjectMgr.hpp"
#include "WorldObject.hpp"

#include "Timer.hpp"

#define _SLEEP_CONST 30
void ObjectMgrRunnable::run()
{
    uint32 realCurrTime = 0;
    uint32 realPrevTime = GetMSTime();
    uint32 prevSleepTime = 0;

    while (!sObjectMgr->IsTerminated())
    {
        realCurrTime = GetMSTime();
        uint32 diff = GetMSTimeDiff(realPrevTime,realCurrTime);
        sObjectMgr->Update(diff);
        realPrevTime = realCurrTime;
        // diff (D0) include time of previous sleep (d0) + tick time (t0)
        // we want that next d1 + t1 == WORLD_SLEEP_CONST
        // we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
        // d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
        if (diff <= _SLEEP_CONST+prevSleepTime)
        {
            prevSleepTime = _SLEEP_CONST+prevSleepTime-diff;
            ACE_Based::Thread::Sleep(prevSleepTime);
        }
        else
            prevSleepTime = 0;
    }
}

ObjectMgr::ObjectMgr()
{
    _terminate = false;
    _WorldObjects.clear();
}

ObjectMgr::~ObjectMgr()
{
    for (std::set<WorldObject*>::iterator itr = _WorldObjects.begin(); itr != _WorldObjects.end(); itr++)
    {
        if ((*itr))
            delete (*itr);
    }
    _WorldObjects.clear();
}

void ObjectMgr::AddObject(WorldObject* object)
{
    _WorldObjects.insert(object);
}

void ObjectMgr::Update(uint32 diff)
{
    for (std::set<WorldObject*>::const_iterator itr = _WorldObjects.begin(); itr != _WorldObjects.end(); itr++)
    {
        if ((*itr) != NULL)
            (*itr)->Update(diff);
    }
}

void ObjectMgr::RemoveObject(WorldObject* object)
{
    for (std::set<WorldObject*>::iterator itr = _WorldObjects.begin(); itr != _WorldObjects.end(); itr++)
    {
        if ((*itr) == object)
        {
            _WorldObjects.erase(itr);
            return;
        }
    }
}

void ObjectMgr::ClearList()
{
    _WorldObjects.clear();
}
