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
};

}
#endif //PUFF_VIDEORENDERER_H
