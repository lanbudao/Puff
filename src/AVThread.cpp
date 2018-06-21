#include "AVThread.h"
#include "AVThread_p.h"

namespace Puff {

AVThread::AVThread(AVThreadPrivate *d):
    CThread(),
    d_ptr(d)
{

}

AVThread::~AVThread() {

}

PacketQueue * AVThread::packets()
{
    DPTR_D(AVThread);
    return &(d->packets);
}

void AVThread::setDecoder(AVDecoder *decoder)
{
    DPTR_D(AVThread);
    d->decoder = decoder;
}

void AVThread::setOutputSet(OutputSet *output)
{
    DPTR_D(AVThread);
    d->output = output;
}

void AVThread::setClock(AVClock *clock)
{
    DPTR_D(AVThread);
    d->clock = clock;
}

AVClock *AVThread::clock()
{
    DPTR_D(AVThread);
    return d->clock;
}

}
