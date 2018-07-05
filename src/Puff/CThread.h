#ifndef PUFF_CTHREAD_H
#define PUFF_CTHREAD_H

#include "AVGlobal.h"
#include "SignalSlot.h"
#include "DPTR.h"

struct SDL_Thread;

namespace Puff {

class CThreadPrivate;
class PU_AV_EXPORT CThread
{
    DPTR_DECLARE_PRIVATE(CThread)
public:
    CThread();
    virtual ~CThread();

    void start();
    virtual void stop();
    int  wait();
    void sleep(int second);
    void msleep(int ms);
    bool isRunning() const;

    unsigned long id() const;

PU_SIGNALS:
    Signal<void*> finished;

public:
    virtual void run();
    virtual void stoped();

protected:
    SDL_Thread *t;

private:
    DPTR_DECLARE(CThread)
};

}
#endif //PUFF_CTHREAD_H
