#include "VideoThread.h"
#include "AVThread_p.h"
#include "VideoDecoder.h"
#include "AVLog.h"
#include "OutputSet.h"

namespace Puff {

class VideoThreadPrivate: public AVThreadPrivate
{
public:
    VideoThreadPrivate() {}
    ~VideoThreadPrivate() {}

    VideoFrame current_frame;
};

VideoThread::VideoThread():
    AVThread(new VideoThreadPrivate)
{

}

VideoThread::~VideoThread()
{

}

void VideoThread::run()
{
    DPTR_D(VideoThread);

    avdebug("video thread id:%d.\n", id());
    auto *dec = dynamic_cast<VideoDecoder *>(d->decoder);
    AVClock *clock = d->clock;
    d->stopped = false;

    Packet pkt;

    while (!d->stopped) {

        if (d->stopped)
            break;
        pkt = d->packets.dequeue();

        if (pkt.isEOF()) {
            sendVideoFrame(VideoFrame());
            break;
        } else {
            if (!pkt.isValid()) {
                continue;
            }
        }

        if (!dec->decode(pkt)) {
            pkt = Packet();
            continue;
        }
        VideoFrame frame = dec->frame();
        if (!frame.isValid()) {
            continue;
        }
        if (!sendVideoFrame(frame))
            continue;
        d->current_frame = frame;
        if (frame.timestamp() > 0 && clock->value() > 0) {
            double delay = frame.timestamp() - clock->value();
            if (delay > 0) {
                msleep((unsigned int)(delay * 1000));
            }
        } else {
            msleep(34);
        }
    }    
    d->packets.clear();
    AVThread::run();
}

bool VideoThread::sendVideoFrame(VideoFrame &frame)
{
    DPTR_D(VideoThread);
    d->output->lock();
    d->output->sendVideoFrame(frame);
    d->output->unlock();
    return true;
}

}
