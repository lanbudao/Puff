#ifndef PUFF_AVOUTPUT_P_H
#define PUFF_AVOUTPUT_P_H

#include "DPTR.h"

namespace Puff {

class AVOutputPrivate: public DptrPrivate<AVOutput>
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
    VideoRendererPrivate()
    {

    }

    ~VideoRendererPrivate()
    {

    }
};

}

#endif //PUFF_AVOUTPUT_P_H
