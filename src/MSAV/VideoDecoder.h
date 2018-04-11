#ifndef MSAV_VIDEODECODER_H
#define MSAV_VIDEODECODER_H

#include "AVDecoder.h"

namespace MSAV {

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
#endif //MSAV_VIDEODECODER_H
