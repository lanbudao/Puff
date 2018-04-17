#include "VideoDecoder.h"
#include "AVDecoder_p.h"

namespace Puff {

class VideoDecoderFFmpegPrivate PU_NO_COPY: public VideoDecoderPrivate
{
public:
    VideoDecoderFFmpegPrivate() {}
    ~VideoDecoderFFmpegPrivate() {}
};

class VideoDecoderFFmpeg: public VideoDecoder
{
public:
    VideoDecoderFFmpeg()
        :VideoDecoder()
    {

    }
    ~VideoDecoderFFmpeg()
    {

    }
};

}
