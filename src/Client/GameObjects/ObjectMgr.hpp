/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __OBJECTMGR_HPP__
#define __OBJECTMGR_HPP__

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include "Threading.hpp"
#include "LockedQueue.hpp"

#include "Common.h"
#include <set>

class WorldObject;

class ObjectMgrRunnable : public ACE_Based::Runnable
{
    public:
        void run();
};

class ObjectMgr
{
    public:
        ObjectMgr();
        ~ObjectMgr();

        void AddObject(WorldObject *object);
        void Update(uint32 diff);
        void RemoveObject(WorldObject *object);
        void ClearList();

        void Terminate() { _terminate = true; };
        bool IsTerminated() { return _terminate; }
    private:
        bool _terminate;
        std::set<WorldObject*>  _WorldObjects;

};
#define sObjectMgr ACE_Singleton<ObjectMgr, ACE_Null_Mutex>::instance()
#endif
