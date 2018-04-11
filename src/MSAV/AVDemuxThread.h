#ifndef MSAV_AVDEMUXTHREAD_H
#define MSAV_AVDEMUXTHREAD_H

#include "CThread.h"

namespace MSAV {

class AVThread;
class AVDemuxer;
class AVDemuxThreadPrivate;
class AVDemuxThread: public CThread
{
    DPTR_DECLARE_PRIVATE(AVDemuxThread)
public:
    AVDemuxThread();
    ~AVDemuxThread() MS_DECL_OVERRIDE;

    void setDemuxer(AVDemuxer *demuxer);
    void setAudioThread(AVThread *thread);
    AVThread *audioThread();
    void setVideoThread(AVThread *thread);
    AVThread *videoThread();

protected:
    void run() MS_DECL_OVERRIDE;

private:
    DPTR_DECLARE(AVDemuxThread)

};

}
#endif //MSAV_AVDEMUXTHREAD_H
