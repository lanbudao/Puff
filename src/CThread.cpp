#include "CThread.h"

namespace MSAV {

CThread::CThread()//: t(NULL)
{
//    t = new boost::thread(run);
//    t->join();
}

CThread::~CThread()
{
//    delete t;
}

void CThread::run()
{

}

int CThread::id() const
{
//    thread::id t_id = t->get_id();
//    return t->get_id();
    return 0;
}

}