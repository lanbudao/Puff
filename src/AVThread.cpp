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

void AVThread::stop()
{
    DPTR_D(AVThread);
    if (d->stopped)
        return;
    d->stopped = true;
    CThread::stop();
}

void AVThread::pause(bool p)
{
    DPTR_D(AVThread);
    if (d->paused == p)
        return;
    d->paused = p;
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

void AVThread::requestSeek()
{
    DPTR_D(AVThread);
    class SeekPTS: public Runnable {
    public:
        SeekPTS(void *p): Runnable(p) {}
        void run() {
            AVThread *p = (AVThread*)context;
            p->d_func()->seek_requested = true;
        }
    };
    d->addTask(new SeekPTS(this));
}

void AVThread::executeNextTask()
{
    DPTR_D(AVThread);
    if (!d->tasks.empty()) {
        Runnable *task = d->tasks.front();
        d->tasks.pop_front();
        if (task)
            task->run();
        delete task;
    }
}

}
