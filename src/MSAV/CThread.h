#ifndef MSAV_CTHREAD_H
#define MSAV_CTHREAD_H

#include "boost/thread.hpp"
#include "boost/bind.hpp"

namespace MSAV {

//namespace boost {
//    class thread;
//}

class CThread
{
public:
    CThread();
    virtual ~CThread();

    virtual void run();

    int id() const;

protected:
//    boost::thread *t;
};

}
#endif //MSAV_CTHREAD_H
