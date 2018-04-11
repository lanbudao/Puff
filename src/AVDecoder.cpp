#include "AVDecoder.h"
#include "AVDecoder_p.h"

namespace MSAV {

AVDecoder::AVDecoder()
{

}

AVDecoder::~AVDecoder()
{

}

AVCodecContext * AVDecoder::codecCtx()
{
    DPTR_D(AVDecoder);
    return d.codec_ctx;
}

}