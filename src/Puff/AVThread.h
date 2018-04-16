#ifndef PUFF_AVTHREAD_H
#define PUFF_AVTHREAD_H

#include "CThread.h"

namespace Puff {

class PacketQueue;
class AVThreadPrivate;
class PU_AV_EXPORT AVThread: public CThread
{
    DPTR_DECLARE_PRIVATE(AVThread)
public:
    AVThread();
    virtual ~AVThread();

    PacketQueue *packets();

protected:
    DPTR_DECLARE(AVThread)
};

}
#endif //PUFF_AVTHREAD_H
