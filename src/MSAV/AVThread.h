#ifndef MSAV_AVTHREAD_H
#define MSAV_AVTHREAD_H

#include "CThread.h"

namespace MSAV {

class PacketQueue;
class AVThreadPrivate;
class MS_AV_EXPORT AVThread: public CThread
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
#endif //MSAV_AVTHREAD_H
