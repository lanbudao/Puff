#include "AVDemuxThread.h"
#include "AVDemuxer.h"
#include "AudioThread.h"
#include "VideoThread.h"

namespace MSAV {

class AVDemuxThreadPrivate: public DptrPrivate<AVDemuxThread>
{
public:
    AVDemuxThreadPrivate():
        demuxer(NULL),
        audio_thread(NULL),
        video_thread(NULL),
        isEnd(false)
    {

    }
    ~AVDemuxThreadPrivate()
    {

    }
    AVDemuxer *demuxer;
    AVThread *audio_thread, *video_thread;
    bool isEnd;
};

AVDemuxThread::AVDemuxThread() {

}

AVDemuxThread::~AVDemuxThread() {

}

void AVDemuxThread::setDemuxer(AVDemuxer *demuxer)
{
    DPTR_D(AVDemuxThread);
    d.demuxer = demuxer;
}

void AVDemuxThread::setAudioThread(AVThread *thread)
{
    DPTR_D(AVDemuxThread);
    d.audio_thread = thread;
}

AVThread *AVDemuxThread::audioThread()
{
    DPTR_D(const AVDemuxThread);
    return d.audio_thread;
}

void AVDemuxThread::setVideoThread(AVThread *thread)
{
    DPTR_D(AVDemuxThread);
    d.video_thread = thread;
}

AVThread *AVDemuxThread::videoThead()
{
    DPTR_D(const AVDemuxThread);
    return d.video_thread;
}

void AVDemuxThread::run()
{
    DPTR_D(AVDemuxThread);

    if (d.audio_thread && !d.audio_thread->isRunning()) {
        d.audio_thread->start();
    }
    if (d.video_thread && !d.video_thread->isRunning()) {
        d.video_thread->start();
    }
    while (!d.isEnd) {

        if (!d.demuxer->readFrame())
            continue;

    }
    CThread::run();
}

}