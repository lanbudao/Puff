#ifndef MSAV_AVDECODER_P_H
#define MSAV_AVDECODER_P_H

#include "DPTR.h"
#include "commpeg.h"

namespace Puff {

class AVDecoderPrivate: public DptrPrivate<AVDecoder>
{
public:
    AVDecoderPrivate():
            codec_ctx(NULL)
    {
        codec_ctx = avcodec_alloc_context3(NULL);
    }
    ~AVDecoderPrivate()
    {
        avcodec_free_context(&codec_ctx);
    }

    AVCodecContext *codec_ctx;
};

}
#endif //MSAV_AVDECODER_P_H
