#ifndef MSAV_CTHREAD_H
#define MSAV_CTHREAD_H

#include "CObject.h"

namespace boost {
    class thread;
}

namespace MSAV {

class MS_AV_EXPORT CThread: public CObject
{
public:
    CThread();
    virtual ~CThread();

    void start();
    void exit();
    void sleep(int second);
    void msleep(int ms);

    int id() const;

MS_SIGNALS:
    signals2::signal<void()> finished;

public:
    virtual void run();

protected:
    boost::thread *t;
};

}
#endif //MSAV_CTHREAD_H
