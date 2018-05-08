#include "CThread.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "AVLog.h"

namespace Puff {

class CThreadPrivate: public DptrPrivate<CThread>
{
public:
    CThreadPrivate(): running(false) {

    }
    ~CThreadPrivate() {

    }

    bool running;
};

CThread::CThread():
    t(NULL)
{
}

CThread::~CThread()
{
    delete t;
}

void CThread::start()
{
    DPTR_D(CThread);
    d.running = true;
    t = new boost::thread(boost::bind(&CThread::run, this));
    t->join();
}

void CThread::exit() {
    DPTR_D(CThread);
    if (t) {
        t->interrupt();
    }
}

void CThread::run()
{
    DPTR_D(CThread);
    avdebug("CThread::finished\n");
    d.running = false;
    PU_EMIT finished();
}

void CThread::sleep(int second)
{
    msleep(second * 1000);
}

void CThread::msleep(int ms)
{
    boost::this_thread::sleep(boost::posix_time::milliseconds(ms));
}

int CThread::id() const
{
//    thread::id t_id = t->get_id();
//    return t->get_id();
    return 0;
}

bool CThread::isRunning() {
    DPTR_D(CThread);
    return d.running;
}

}
