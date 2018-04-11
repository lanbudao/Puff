#ifndef MSAV_CTHREAD_H
#define MSAV_CTHREAD_H

#include "CObject.h"

namespace boost {class thread;}

namespace MSAV {

class CThreadPrivate;
class MS_AV_EXPORT CThread: public CObject
{
    DPTR_DECLARE_PRIVATE(CThread)
public:
    CThread();
    virtual ~CThread();

    void start();
    void exit();
    void sleep(int second);
    void msleep(int ms);
    bool isRunning();

    int id() const;

MS_SIGNALS:
    signals2::signal<void()> finished;

public:
    virtual void run();

protected:
    boost::thread *t;

private:
    DPTR_DECLARE(CThread)
};

}
#endif //MSAV_CTHREAD_H
