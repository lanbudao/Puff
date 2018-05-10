#include "CThread.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
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
//    t->detach();
}

void CThread::stop() {
    DPTR_D(CThread);
    if (t) {
        t->interrupt();
        t->join();
    }
    avdebug("CThread::stoped\n");
    d.running = false;
    PU_EMIT finished();
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

unsigned long CThread::id() const
{
    String id;
    id = boost::lexical_cast<String>(boost::this_thread::get_id());
    return std::stoul(id, NULL, 16);;
}

bool CThread::isRunning() const {
    DPTR_D(const CThread);
    return d.running;
}

}
