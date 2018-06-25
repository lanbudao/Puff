#ifndef PUFF_AVDEMUXTHREAD_H
#define PUFF_AVDEMUXTHREAD_H

#include "CThread.h"

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
    void setDemuxer(AVDemuxer *demuxer);
    void setAudioThread(AVThread *thread);
    AVThread *audioThread();
    void setVideoThread(AVThread *thread);
    AVThread *videoThread();

protected:
    void run() PU_DECL_OVERRIDE;

private:
    DPTR_DECLARE(AVDemuxThread)

};

}
#endif //PUFF_AVDEMUXTHREAD_H
