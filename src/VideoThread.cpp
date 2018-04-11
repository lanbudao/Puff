#include "VideoThread.h"
#include "AVThread_p.h"
#include "VideoDecoder.h"

namespace MSAV {

class VideoThreadPrivate: public AVThreadPrivate
{
public:
    VideoThreadPrivate() {}
    ~VideoThreadPrivate() {}
};

VideoThread::VideoThread()//: AVThread()
{

}

VideoThread::~VideoThread()
{

}

void VideoThread::run()
{
    DPTR_D(VideoThread);

    auto *decoder = dynamic_cast<VideoDecoder *>(d.decoder);

    while (!d.stopped) {

    }

    AVThread::run();
}

}