#include "VideoFrame.h"
#include "Frame_p.h"

namespace Puff {

class VideoFramePrivate : public FramePrivate {
public:
    VideoFramePrivate():
        width(0),
        height(0),
        color_space(ColorSpace_Unknown),
        color_range(ColorRange_Unknown),
        displayAspectRatio(1.0)
    {

    }

    void setFormat(const VideoFormat &fmt)
    {
        format = fmt;
        planes.reserve(fmt.planeCount());
        planes.resize(fmt.planeCount());
        line_sizes.reserve(fmt.planeCount());
        line_sizes.resize(fmt.planeCount());
    }
    VideoFormat format;
    int width, height;

    ColorSpace color_space;
    ColorRange color_range;

    float displayAspectRatio;
};

VideoFrame::VideoFrame():
    Frame()
{

}

VideoFrame::VideoFrame(int width, int height, const VideoFormat &format, const ByteArray &data):
    Frame()
{
    DPTR_D(VideoFrame);
    d.width = width;
    d.height = height;
    d.data = data;
    d.setFormat(format);
}

VideoFrame::~VideoFrame()
{

}

int VideoFrame::channelCount() const {
    DPTR_D(const VideoFrame);
    if (!d.format.isValid())
        return 0;
    return d.format.channels();
}

bool VideoFrame::isValid() const {
    DPTR_D(const VideoFrame);
    return d.width > 0 && d.height > 0 && d.format.isValid();
}

Size VideoFrame::size() {
    DPTR_D(const VideoFrame);
    return Size(d.width, d.height);
}

VideoFormat VideoFrame::format() const {
    DPTR_D(const VideoFrame);
    return d.format;
}

VideoFormat::EPixelFormat VideoFrame::pixelFormat() const {
    DPTR_D(const VideoFrame);
    return d.format.pixelFormat();
}

int VideoFrame::pixelFormatFFmpeg() const {
    DPTR_D(const VideoFrame);
    return d.format.pixelFormatFFmpeg();
}

int VideoFrame::width() const {
    DPTR_D(const VideoFrame);
    return d.width;
}

int VideoFrame::height() const {
    DPTR_D(const VideoFrame);
    return d.height;
}

ColorSpace VideoFrame::colorSpace() const {
    DPTR_D(const VideoFrame);
    return d.color_space;
}

void VideoFrame::setColorSpace(ColorSpace space) {
    DPTR_D(VideoFrame);
    d.color_space = space;
}

ColorRange VideoFrame::colorRange() const {
    DPTR_D(const VideoFrame);
    return d.color_range;
}

void VideoFrame::setColorRange(ColorRange range) {
    DPTR_D(VideoFrame);
    d.color_range = range;
}

int VideoFrame::planeWidth(int plane) const {
    DPTR_D(const VideoFrame);
    return d.format.width(d.width, plane);
}

int VideoFrame::planeHeight(int plane) const {
    DPTR_D(const VideoFrame);
    return d.format.height(d.height, plane);
}

void VideoFrame::setDisplayAspectRatio(float aspect) {
    DPTR_D(VideoFrame);
    d.displayAspectRatio = aspect;
}

}
