#ifndef PUFF_VIDEODECODERFFMPEGBASE_H
#define PUFF_VIDEODECODERFFMPEGBASE_H

#include "VideoDecoder.h"

namespace Puff {

class VideoDecoderFFmpegBasePrivate;
class VideoDecoderFFmpegBase: public VideoDecoder
{
    DPTR_DECLARE_PRIVATE(VideoDecoderFFmpegBase)
public:
    VideoDecoderFFmpegBase(VideoDecoderFFmpegBasePrivate *d);
    ~VideoDecoderFFmpegBase();

    virtual bool decode(const Packet &pkt);
    VideoFrame frame();

private:

};

}
#endif //PUFF_VIDEODECODERFFMPEGBASE_H
