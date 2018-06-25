#include "VideoRender_p.h"

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

void VideoRenderer::resizeWindow(const Size &size) {
    resizeWindow(size.width(), size.height());
}

void VideoRenderer::resizeWindow(int width, int height) {
    DPTR_D(VideoRenderer);
    d->window_width = width;
    d->window_height = height;
    onResizeWindow(width, height);
}

void VideoRenderer::onResizeWindow(int width, int height) {
    PU_UNUSED(width)
    PU_UNUSED(height)
}

}
