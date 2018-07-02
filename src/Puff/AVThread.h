#ifndef PUFF_AVTHREAD_H
#define PUFF_AVTHREAD_H

#include "CThread.h"

namespace Puff {

class AVClock;
class OutputSet;
class PacketQueue;
class AVDecoder;
class AVThreadPrivate;
class PU_AV_EXPORT AVThread: public CThread
{
    DPTR_DECLARE_PRIVATE(AVThread)
public:
    virtual ~AVThread();

    void stop() PU_DECL_OVERRIDE;
    void pause(bool p);

    PacketQueue *packets();

    void setDecoder(AVDecoder *decoder);

    void setOutputSet(OutputSet *output);

    void setClock(AVClock *clock);
    AVClock *clock();

    void requestSeek();

    void executeNextTask();

    bool isSeeking() const;
    void setSeeking(bool s);

protected:
    AVThread(AVThreadPrivate *d);
    DPTR_DECLARE(AVThread)
};

}
#endif //PUFF_AVTHREAD_H
