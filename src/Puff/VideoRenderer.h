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
    virtual ~VideoRenderer();

    void receive(const VideoFrame &frame);

    void resizeRenderer(const Size &size);
    void resizeRenderer(int width, int height);

protected:
    virtual bool receiveFrame(const VideoFrame& frame) = 0;
    virtual void onResizeRenderer(int width, int height);
    VideoRenderer(VideoRendererPrivate *d);
};

}
#endif //PUFF_VIDEORENDERER_H
