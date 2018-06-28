#include "AVDemuxThread.h"
#include "AVDemuxer.h"
#include "AudioThread.h"
#include "VideoThread.h"
#include "PacketQueue.h"
#include "AVLog.h"
#include "AVClock.h"

namespace Puff {

class AVDemuxThreadPrivate
{
public:
    AVDemuxThreadPrivate():
        demuxer(NULL),
        audio_thread(NULL),
        video_thread(NULL),
        stopped(false),
        paused(false),
        seek_req(false),
        clock(NULL)
    {

    }
    ~AVDemuxThreadPrivate()
    {

    }
    AVDemuxer *demuxer;
    AVThread *audio_thread, *video_thread;
    bool stopped;
    bool paused;

    bool seek_req; uint64_t seek_pos; SeekType seek_type;
    AVClock *clock;
};

AVDemuxThread::AVDemuxThread():
    d_ptr(new AVDemuxThreadPrivate)
{

}

AVDemuxThread::~AVDemuxThread() {

}

void AVDemuxThread::stop()
{
    DPTR_D(AVDemuxThread);
    d->stopped = true;
    CThread::stop();
    if (d->audio_thread)
        d->audio_thread->stop();
    if (d->video_thread)
        d->video_thread->stop();
}

void AVDemuxThread::pause(bool p)
{
    DPTR_D(AVDemuxThread);
    if (d->paused == p)
        return;
    d->paused = p;
}

void AVDemuxThread::seek(uint64_t ms, SeekType type)
{
    DPTR_D(AVDemuxThread);
    d->seek_req = true;
    d->seek_pos = ms;
    d->seek_type = type;
}

void AVDemuxThread::setDemuxer(AVDemuxer *demuxer)
{
    DPTR_D(AVDemuxThread);
    d->demuxer = demuxer;
}

void AVDemuxThread::setAudioThread(AVThread *thread)
{
    DPTR_D(AVDemuxThread);
    d->audio_thread = thread;
}

AVThread *AVDemuxThread::audioThread()
{
    DPTR_D(const AVDemuxThread);
    return d->audio_thread;
}

void AVDemuxThread::setVideoThread(AVThread *thread)
{
    DPTR_D(AVDemuxThread);
    d->video_thread = thread;
}

AVThread *AVDemuxThread::videoThread()
{
    DPTR_D(const AVDemuxThread);
    return d->video_thread;
}

void AVDemuxThread::setClock(AVClock *clock)
{
    d_func()->clock = clock;
}

void AVDemuxThread::seekInternal(uint64_t pos, SeekType type)
{
    DPTR_D(AVDemuxThread);

    AVThread* av[] = { d->audio_thread, d->video_thread};

    if (d->audio_thread) {
        d->audio_thread->packets()->clear();
    }
    if (d->video_thread) {
        d->video_thread->packets()->clear();
    }
    d->demuxer->setSeekType(type);
    d->demuxer->seek(pos);

    for (size_t i = 0; i < sizeof(av) / sizeof(av[0]); ++i) {
        AVThread *t = av[i];
        if (!t) continue;
        t->packets()->setBlock(false);
        t->requestSeek();
        Packet pkt;
        pkt.pts = pos / 1000.0;
        pkt.pos = pos;
        t->packets()->enqueue(pkt);
        t->packets()->setBlock(true);
    }
//    d->clock->updateValue((double)pos / 1000);
    seekFinished(NULL);
}

void AVDemuxThread::run()
{
    DPTR_D(AVDemuxThread);
    int stream;
    Packet pkt;
    int ret = -1;

    if (d->audio_thread && !d->audio_thread->isRunning()) {
        d->audio_thread->start();
    }
    if (d->video_thread && !d->video_thread->isRunning()) {
        d->video_thread->start();
    }
    PacketQueue *vbuffer = d->video_thread ? d->video_thread->packets() : NULL;
    PacketQueue *abuffer = d->audio_thread ? d->audio_thread->packets() : NULL;

    if (vbuffer) {
        vbuffer->clear();
        vbuffer->setBlock(true);
    }
    if (abuffer) {
        abuffer->clear();
        abuffer->setBlock(true);
    }
    bool audio_has_pic = false;
    d->stopped = false;
    bool enqueue_eof = false;

    while (true) {
        if (d->seek_req) {
            seekInternal(d->seek_pos, d->seek_type);
            d->seek_req = false;
        }
        if (d->stopped)
            break;
        if (d->paused) {
            msleep(1);
            continue;
        }
        if (d->demuxer->atEnd()) {
            // wait for a/v thread finished
            if (!enqueue_eof) {
                if (abuffer) {
                    abuffer->enqueue(Packet::createEOF());
                    abuffer->blockEmpty(false);
                }
                if (vbuffer) {
                    vbuffer->enqueue(Packet::createEOF());
                    vbuffer->blockEmpty(false);
                }
                enqueue_eof = true;
            }
            if (abuffer->isEmpty() && vbuffer->isEmpty()) {
                break;
            }
            msleep(10);
            continue;
        }
        audio_has_pic = d->demuxer->hasAttachedPic();

        ret = d->demuxer->readFrame();
        if (ret < 0)
            continue;
        stream = d->demuxer->stream();
        pkt = d->demuxer->packet();

        if (stream == d->demuxer->videoStream()) {
            if (vbuffer) {
                if (!d->video_thread || !d->video_thread->isRunning()) {
                    vbuffer->clear();
                    continue;
                }
                vbuffer->blockFull(!d->audio_thread || !d->audio_thread->isRunning() || !abuffer || abuffer->isEnough());
                vbuffer->enqueue(pkt);
            }
        }
        else if (stream == d->demuxer->audioStream()) {
            if (abuffer) {
                if (!d->audio_thread || !d->audio_thread->isRunning()) {
                    abuffer->clear();
                    continue;
                }
                abuffer->blockFull(!d->video_thread || !d->video_thread->isRunning() || !vbuffer || audio_has_pic);
                abuffer->enqueue(pkt);
            }
        }
        else if (stream == d->demuxer->subtitleStream()) {

        }
        msleep(1);
    }

    CThread::run();
}

}
