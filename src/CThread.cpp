#include "CThread.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"

namespace MSAV {

CThread::CThread(): t(NULL)
{
    t = new boost::thread(boost::bind(&CThread::run, this));
}

CThread::~CThread()
{
    delete t;
}

void CThread::start()
{
    t->join();
}

void CThread::exit() {
    t->interrupt();
}

void CThread::run()
{
    printf("run...\n");

    MS_EMIT finished();
}

void CThread::sleep(int second)
{
    boost::this_thread::sleep(boost::posix_time::seconds(second));
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

}