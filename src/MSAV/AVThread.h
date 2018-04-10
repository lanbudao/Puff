#ifndef MSAV_AVTHREAD_H
#define MSAV_AVTHREAD_H

#include "CThread.h"

namespace MSAV {

class MS_AV_EXPORT AVThread: public CThread
{
public:
    AVThread();
    virtual ~AVThread();
    
};

}
#endif //MSAV_AVTHREAD_H
