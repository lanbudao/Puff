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
    VideoRenderer();
    virtual ~VideoRenderer();

    void receiveFrame(const VideoFrame &frame);

    void resizeRenderer(const Size &size);
    void resizeRenderer(int width, int height);

protected:
    virtual void onResizeRenderer(int width, int height);
};

}
#endif //PUFF_VIDEORENDERER_H
