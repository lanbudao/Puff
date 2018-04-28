#ifndef PUFF_VIDEOFRAME_H
#define PUFF_VIDEOFRAME_H

#include "VideoFormat.h"
#include "Frame.h"
#include "Size.h"

#include <boost/range/size.hpp>

namespace Puff {

class VideoFramePrivate;
class PU_AV_EXPORT VideoFrame: public Frame
{
    DPTR_DECLARE_PRIVATE(VideoFrame)
public:
    VideoFrame();
    virtual ~VideoFrame();

    Size size();

private:

};


}
#endif //PUFF_VIDEOFRAME_H
