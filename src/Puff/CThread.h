#ifndef PUFF_CTHREAD_H
#define PUFF_CTHREAD_H

#include "CObject.h"

namespace boost {class thread;}

namespace Puff {

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

PU_SIGNALS:
    signals2::signal<void()> finished;

public:
    virtual void run();

protected:
    boost::thread *t;

private:
    DPTR_DECLARE(CThread)
};

}
#endif //PUFF_CTHREAD_H
