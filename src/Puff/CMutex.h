#ifndef PUFF_CMUTEX_H
#define PUFF_CMUTEX_H

#include "DPTR.h"

struct SDL_mutex;

namespace Puff {

#define DeclReadLockMutex(x) ReadLock lock(x);(void)lock;
#define DeclWriteLockMutex(x) WriteLock lock(x);(void)lock;
class CMutexPrivate;
class CMutex
{
    DPTR_DECLARE_PRIVATE(CMutex)
public:
    CMutex();
    ~CMutex();

    bool readLock();
    bool readUnlock();
    bool writeLock();
    bool writeUnlock();

    SDL_mutex *mutex();
private:
    DPTR_DECLARE(CMutex)
};

class ReadLock
{
public:
    ReadLock(CMutex *mtx);
    ~ReadLock();
private:
    CMutex *mutex;
};

class WriteLock
{
public:
    WriteLock(CMutex *mtx);
    ~WriteLock();
private:
    CMutex *mutex;
};

class CConditionPrivate;
class CCondition
{
    DPTR_DECLARE_PRIVATE(CCondition)
public:
    CCondition();
    ~CCondition();

    void notifyOne();
    void notifyAll();
    void timeWait(CMutex *mutex, int timeout = ULONG_MAX);

private:
    DPTR_DECLARE(CCondition)
};

}
#endif //PUFF_CMUTEX_H
