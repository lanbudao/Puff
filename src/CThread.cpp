#include "CThread.h"
#include "AVLog.h"
#include "SDL_thread.h"
#include "SDL_timer.h"
#include "util.h"

namespace Puff {

static int running(void *context)
{
    CThread *thread = static_cast<CThread *>(context);
    if (thread)
        thread->run();
    return 0;
}

class CThreadPrivate
{
public:
    CThreadPrivate():
        running(false)
    {
    }
    ~CThreadPrivate() {

    }


    bool running;
};

CThread::CThread():
    d_ptr(new CThreadPrivate),
    t(NULL)
{
}

CThread::~CThread()
{
}

void CThread::start()
{
    DPTR_D(CThread);
    d->running = true;
    t = SDL_CreateThread(running, Util::guid().c_str(), this);
}

void CThread::stop() {
    DPTR_D(CThread);
    stoped();
    int status;
    SDL_WaitThread(t, &status);
    avdebug("thread stoped!\n");
    d->running = false;
}

int CThread::wait()
{
    int status;
    SDL_WaitThread(t, &status);
    return status;
}

void CThread::run()
{
    DPTR_D(CThread);
    avdebug("Thread %d finished!\n", id());
    d->running = false;
    PU_EMIT finished(NULL);
}

void CThread::stoped()
{

}

void CThread::sleep(int second)
{
    msleep(second * 1000);
}

void CThread::msleep(int ms)
{
    SDL_Delay(ms);
}

unsigned long CThread::id() const
{
    return (unsigned long)SDL_GetThreadID(t);
}

bool CThread::isRunning() const {
    DPTR_D(const CThread);
    return d->running;
}

}
