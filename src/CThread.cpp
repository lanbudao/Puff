#include "CThread.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"

namespace MSAV {

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
    t = new boost::thread(boost::bind(&CThread::run, this));
}

CThread::~CThread()
{
    delete t;
}

void CThread::start()
{
    DPTR_D(CThread);
    t->join();
    d.running = true;
}

void CThread::exit() {
    DPTR_D(CThread);
    t->interrupt();
}

void CThread::run()
{
    DPTR_D(CThread);
    printf("CThread::finished\n");
    d.running = false;
    MS_EMIT finished();
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