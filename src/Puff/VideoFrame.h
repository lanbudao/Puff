#ifndef PUFF_VIDEOFRAME_H
#define PUFF_VIDEOFRAME_H

#include "VideoFormat.h"
#include "Frame.h"
#include "Size.h"

namespace Puff {

class VideoFramePrivate;
class PU_AV_EXPORT VideoFrame: public Frame
{
    DPTR_DECLARE_PRIVATE(VideoFrame)
public:
    VideoFrame();
    VideoFrame(const VideoFrame &other);
    VideoFrame(int width, int height, const VideoFormat &format, const ByteArray &data = ByteArray());
    virtual ~VideoFrame();
    VideoFrame &operator=(const VideoFrame &other);

    int channelCount() const;

    VideoFormat format() const;

    VideoFormat::EPixelFormat pixelFormat() const;
    int pixelFormatFFmpeg() const;

    bool isValid() const;

    Size size();
    int width() const;
    int height() const;

    ColorSpace colorSpace() const;
    void setColorSpace(ColorSpace space);

    ColorRange colorRange() const;
    void setColorRange(ColorRange range);

    int planeWidth(int plane) const;
    int planeHeight(int plane) const;

    void setDisplayAspectRatio(float aspect);

private:
};


}
#endif //PUFF_VIDEOFRAME_H
