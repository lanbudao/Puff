#ifndef PUFF_VIDEODECODER_H
#define PUFF_VIDEODECODER_H

#include "AVDecoder.h"

namespace Puff {

typedef int VideoDecoderId;

class VideoDecoderPrivate;
class PU_AV_EXPORT VideoDecoder: public AVDecoder
{
    DISABLE_COPY(VideoDecoder)
    DPTR_DECLARE_PRIVATE(VideoDecoder)
public:
    static StringList supportedCodecs();
    virtual String name() const;
    virtual String description() const;

private:
    VideoDecoder();
    ~VideoDecoder();
};

extern PU_AV_EXPORT VideoDecoderId VideoDecoderId_FFmpeg;

}
#endif //PUFF_VIDEODECODER_H
