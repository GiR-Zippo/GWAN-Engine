/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOCKEDQUEUE_HPP
#define LOCKEDQUEUE_HPP

#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <deque>
#include <assert.h>

template <class T, class LockType, typename StorageType=std::deque<T> > class LockedQueue
{
public:
    LockedQueue() : _canceled(false) { }
    
    void lock()
    {
        this->_lock.acquire();
    }

    void unlock()
    {
        this->_lock.release();
    }

    void add(const T& item)
    {
        lock();

        _queue.push_back(item);

        unlock();
    }

    bool next(T& result)
    {
        ACE_GUARD_RETURN (LockType, g, this->_lock, false);

        if (_queue.empty())
            return false;

        result = _queue.front();
        _queue.pop_front();

        return true;
    }

    T& peek(bool autoUnlock = false)
    {
        lock();

        T& result = _queue.front();

        if (autoUnlock)
            unlock();

        return result;
    }

    void pop_front()
    {
        ACE_GUARD (LockType, g, this->_lock);
        _queue.pop_front();
    }

    bool empty()
    {
        ACE_GUARD_RETURN (LockType, g, this->_lock, false);
        return _queue.empty();
    }

private:
    LockType _lock;
    StorageType _queue;

    volatile bool _canceled;
};

#endif
