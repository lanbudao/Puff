#include "VideoThread.h"
#include "AVThread_p.h"

namespace MSAV {

class VideoThreadPrivate: public AVThreadPrivate
{
public:
    VideoThreadPrivate() {}
    ~VideoThreadPrivate() {}
};

VideoThread::VideoThread()
{

}

VideoThread::~VideoThread()
{

}

void VideoThread::run()
{
    DPTR_D(VideoThread);

    while (true) {

    }
}

}