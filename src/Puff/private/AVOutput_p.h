#ifndef PUFF_AVOUTPUT_P_H
#define PUFF_AVOUTPUT_P_H

#include "DPTR.h"
#include "size.h"

namespace Puff {

class AVOutputPrivate
{
public:
    AVOutputPrivate():
        avaliable(false),
        pause(false)
    {

    }
    virtual ~AVOutputPrivate()
    {

    }

    bool avaliable;
    bool pause;
};

class VideoRendererPrivate: public AVOutputPrivate
{
public:
    VideoRendererPrivate():
        renderer_width(0),
        renderer_height(0)
    {

    }

    ~VideoRendererPrivate()
    {

    }

    int renderer_width, renderer_height;
};

}

#endif //PUFF_AVOUTPUT_P_H
