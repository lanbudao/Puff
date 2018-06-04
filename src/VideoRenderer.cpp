#include "VideoRenderer.h"
#include "AVOutput_p.h"

namespace Puff {

VideoRenderer::VideoRenderer(VideoRendererPrivate *d):
    AVOutput(d)
{

}

VideoRenderer::~VideoRenderer()
{

}

void VideoRenderer::setBackgroundColor(int r, int g, int b)
{

}

void VideoRenderer::receive(const VideoFrame &frame)
{
    receiveFrame(frame);
}

void VideoRenderer::resizeRenderer(const Size &size) {
    resizeRenderer(size.width(), size.height());
}

void VideoRenderer::resizeRenderer(int width, int height) {
    DPTR_D(VideoRenderer);
    d->renderer_width = width;
    d->renderer_height = height;
    onResizeRenderer(width, height);
}

void VideoRenderer::onResizeRenderer(int width, int height) {
    PU_UNUSED(width)
    PU_UNUSED(height)
}

}
