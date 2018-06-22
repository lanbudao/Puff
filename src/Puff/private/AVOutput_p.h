#ifndef PUFF_AVOUTPUT_P_H
#define PUFF_AVOUTPUT_P_H

#include "DPTR.h"
#include "size.h"
#include "Statistics.h"

namespace Puff {

class AVOutputPrivate
{
public:
    AVOutputPrivate():
        avaliable(true),
        pause(false),
        statistics(NULL)
    {

    }
    virtual ~AVOutputPrivate()
    {

    }

    bool avaliable;
    bool pause;
    Statistics *statistics;
};

class VideoRendererPrivate: public AVOutputPrivate
{
public:
    VideoRendererPrivate():
        window_width(0),
        window_height(0),
        renderer_width(0),
        renderer_height(0)
    {

    }

    ~VideoRendererPrivate()
    {

    }

    void calculateRendererSize(const Size &frameSize)
    {
        double aspect_ratio = statistics->video_only.aspect_ratio;
        if (aspect_ratio < 0.00001) {
            if (frameSize.hasNone()) {
                renderer_width = renderer_height = 0;
                return;
            }
            aspect_ratio = (double)frameSize.width() / (double)frameSize.height();
        }
        renderer_width = window_width;
        renderer_height = renderer_width / aspect_ratio;
        if (renderer_height > window_height) {
            renderer_height = window_height;
            renderer_width = renderer_height * aspect_ratio;
        }
    }

    int window_width, window_height;
    int renderer_width, renderer_height;
};

}

#endif //PUFF_AVOUTPUT_P_H
