#include "VideoDecoderFFmpegBase.h"
#include "AVDecoder_p.h"

namespace Puff {

class VideoDecoderFFmpegBasePrivate: public VideoDecoderPrivate
{
public:
    VideoDecoderFFmpegBasePrivate():
        frame(NULL)
    {
        avcodec_register_all();
        frame = av_frame_alloc();
    }
    ~VideoDecoderFFmpegBasePrivate()
    {
        if (frame) {
            av_frame_free(&frame);
            frame = NULL;
        }
    }

    AVFrame *frame;
};

VideoDecoderFFmpegBase::VideoDecoderFFmpegBase():
    VideoDecoder()
{
}

VideoDecoderFFmpegBase::~VideoDecoderFFmpegBase()
{
}

bool VideoDecoderFFmpegBase::decode(const Packet &pkt)
{
    DPTR_D(VideoDecoderFFmpegBase);

    if (!isAvailable())
        return false;

    return false;
}

VideoFrame VideoDecoderFFmpegBase::frame() const
{
    DPTR_D(VideoDecoderFFmpegBase);
    return VideoFrame();
}

}