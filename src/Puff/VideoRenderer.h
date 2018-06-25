#ifndef PUFF_VIDEORENDERER_H
#define PUFF_VIDEORENDERER_H

#include "AVOutput.h"
#include "VideoFrame.h"

namespace Puff {

class VideoRendererPrivate;
class PU_AV_EXPORT VideoRenderer: public AVOutput
{
    DPTR_DECLARE_PRIVATE(VideoRenderer)
public:

    enum OutAspectRatioMode {
        RendererAspectRatio,
        VideoAspectRatio,
        CustomAspectRation
    };

    virtual ~VideoRenderer();

    virtual void setBackgroundColor(int r, int g, int b);

    void receive(const VideoFrame &frame);

    void resizeWindow(const Size &size);
    void resizeWindow(int width, int height);

protected:
    virtual bool receiveFrame(const VideoFrame& frame) = 0;
    virtual void onResizeWindow(int width, int height);
    VideoRenderer(VideoRendererPrivate *d);
};

}
#endif //PUFF_VIDEORENDERER_H
