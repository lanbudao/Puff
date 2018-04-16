#ifndef PUFF_VIDEODECODER_H
#define PUFF_VIDEODECODER_H

#include "AVDecoder.h"

namespace Puff {

class VideoDecoderPrivate;
class VideoDecoder: public AVDecoder
{
    DPTR_DECLARE_PRIVATE(VideoDecoder)
public:
    VideoDecoder();
    ~VideoDecoder();

    static StringList supportedCodecs();

private:

};

}
#endif //PUFF_VIDEODECODER_H
