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

    int channelCount() const;

    VideoFormat format() const;

    VideoFormat::PixelFormat pixelFormat() const;

    Size size();
    int width() const;
    int height() const;

    ColorSpace colorSpace() const;
    void setColorSpace(ColorSpace space);

    ColorRange colorRange() const;
    void setColorRange(ColorRange range);

    int planeWidth(int plane) const;
    int planeHeight(int plane) const;

private:

};


}
#endif //PUFF_VIDEOFRAME_H
