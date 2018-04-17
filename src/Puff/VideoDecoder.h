#ifndef PUFF_VIDEODECODER_H
#define PUFF_VIDEODECODER_H

#include "AVDecoder.h"

namespace Puff {

class VideoDecoderPrivate;
class PU_AV_EXPORT VideoDecoder: public AVDecoder
{
    DPTR_DECLARE_PRIVATE(VideoDecoder)
public:
    VideoDecoder();
    ~VideoDecoder();

    static StringList supportedCodecs();
    virtual String name() const;
    virtual String description() const;

private:

};

}
#endif //PUFF_VIDEODECODER_H
