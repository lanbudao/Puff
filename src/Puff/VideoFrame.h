#ifndef PUFF_VIDEOFRAME_H
#define PUFF_VIDEOFRAME_H

#include "Frame.h"

namespace Puff {

class VideoFramePrivate;
class PU_AV_EXPORT VideoFrame: public Frame
{
    DPTR_DECLARE_PRIVATE(VideoFrame)
public:
    VideoFrame();
    virtual ~VideoFrame();

private:

};


}
#endif //PUFF_VIDEOFRAME_H
