#ifndef PUFF_AVTHREAD_H
#define PUFF_AVTHREAD_H

#include "CThread.h"

namespace Puff {

class OutputSet;
class PacketQueue;
class AVDecoder;
class AVThreadPrivate;
class PU_AV_EXPORT AVThread: public CThread
{
    DPTR_DECLARE_PRIVATE(AVThread)
public:
    virtual ~AVThread();

    PacketQueue *packets();

    void setDecoder(AVDecoder *decoder);

    void setOutputSet(OutputSet *output);

protected:
    AVThread(AVThreadPrivate *d);
    DPTR_DECLARE(AVThread)
};

}
#endif //PUFF_AVTHREAD_H
