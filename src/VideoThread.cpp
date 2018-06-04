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

    Packet pkt;

    while (!d->stopped) {

//        if (!pkt.isValid()) {
            pkt = d->packets.dequeue();
//        }

        if (pkt.isEOF()) {

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
        msleep(50);
    }

    AVThread::run();
}

bool VideoThread::sendVideoFrame(VideoFrame &frame)
{
    DPTR_D(VideoThread);
    d->output->sendVideoFrame(frame);
    return true;
}

}
