#include "VideoThread.h"
#include "AVThread_p.h"
#include "VideoDecoder.h"

namespace Puff {

class VideoThreadPrivate: public AVThreadPrivate
{
public:
    VideoThreadPrivate() {}
    ~VideoThreadPrivate() {}

    VideoFrame current_frame;
};

VideoThread::VideoThread():
    AVThread()
{

}

VideoThread::~VideoThread()
{

}

void VideoThread::run()
{
    DPTR_D(VideoThread);

    auto *dec = dynamic_cast<VideoDecoder *>(d.decoder);

    Packet pkt;
    VideoFrame frame;

    while (!d.stopped) {

        if (!pkt.isValid()) {
            pkt = d.packets.dequeue();
        }

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
        frame = dec->frame();
        if (!frame.isValid()) {
            continue;
        }
        if (!sendVideoFrame(frame))
            continue;
        d.current_frame = frame;
    }

    AVThread::run();
}

bool VideoThread::sendVideoFrame(VideoFrame &frame)
{
    DPTR_D(VideoFrame);

    return true;
}

}
