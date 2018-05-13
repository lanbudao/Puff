#include "CMutex.h"
#include "SDL_mutex.h"

namespace Puff {

class CMutexPrivate: public DptrPrivate<CMutex>
{
public:
    CMutexPrivate():
        mutex(NULL),
        sem(NULL),
        readerCount(0)
    {
        sem = SDL_CreateSemaphore(1);
        if (!sem)
            return;
        mutex = SDL_CreateMutex();
        if (!mutex) {
            SDL_DestroySemaphore(sem);
        }
    }
    ~CMutexPrivate()
    {
        if (mutex) {
            SDL_DestroyMutex(mutex);
        }
        if (sem) {
            SDL_DestroySemaphore(sem);
        }
    }

    SDL_mutex *mutex;
    SDL_sem *sem;
    int readerCount;
};

CMutex::CMutex()
{

}

CMutex::~CMutex()
{

}

bool CMutex::readLock()
{
    DPTR_D(CMutex);

    if (SDL_LockMutex(d.mutex) != 0) {
        return false;
    }
    if (d.readerCount++ == 1) {
        if (SDL_SemWait(d.sem) != 0) {
            SDL_UnlockMutex(d.mutex);
            return false;
        }
    }
    SDL_UnlockMutex(d.mutex);
    return true;
}

bool CMutex::readUnlock()
{
    DPTR_D(CMutex);

    if (SDL_LockMutex(d.mutex) != 0) {
        return false;
    }
    if (d.readerCount-- == 0) {
        if (SDL_SemPost(d.sem) != 0) {
            SDL_UnlockMutex(d.mutex);
            return false;
        }
    }
    SDL_UnlockMutex(d.mutex);
    return true;
}

bool CMutex::writeLock()
{
    DPTR_D(CMutex);
    if (SDL_SemWait(d.sem) != 0)
        return false;
    return true;
}

bool CMutex::writeUnlock()
{
    DPTR_D(CMutex);
    if (SDL_SemPost(d.sem) != 0)
        return false;
    return true;
}

}
