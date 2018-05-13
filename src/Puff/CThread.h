#ifndef PUFF_CTHREAD_H
#define PUFF_CTHREAD_H

#include "CObject.h"

namespace Puff {

struct SDL_Thread;
class CThreadPrivate;
class PU_AV_EXPORT CThread: public CObject
{
    DPTR_DECLARE_PRIVATE(CThread)
public:
    CThread();
    virtual ~CThread();

    void start();
    void stop();
    void sleep(int second);
    void msleep(int ms);
    bool isRunning() const;

    unsigned long id() const;

public:
    virtual void run();
    virtual void end(unsigned code);

protected:
    SDL_Thread *t;
private:
    DPTR_DECLARE(CThread)
};

}
#endif //PUFF_CTHREAD_H
