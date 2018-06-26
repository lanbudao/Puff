#ifndef PUFF_AVDEMUXTHREAD_H
#define PUFF_AVDEMUXTHREAD_H

#include "CThread.h"
#include "SignalSlot.h"

namespace Puff {

class AVThread;
class AVDemuxer;
class AVDemuxThreadPrivate;
class AVDemuxThread: public CThread
{
    DPTR_DECLARE_PRIVATE(AVDemuxThread)
public:
    AVDemuxThread();
    ~AVDemuxThread() PU_DECL_OVERRIDE;

    void stop() PU_DECL_OVERRIDE;
    void pause(bool p);
    void seek(uint64_t ms, SeekType type);
    void setDemuxer(AVDemuxer *demuxer);
    void setAudioThread(AVThread *thread);
    AVThread *audioThread();
    void setVideoThread(AVThread *thread);
    AVThread *videoThread();

PU_SIGNALS:
    Signal<void*> seekFinished;

protected:
    void run() PU_DECL_OVERRIDE;

private:
    DPTR_DECLARE(AVDemuxThread)

};

}
#endif //PUFF_AVDEMUXTHREAD_H
