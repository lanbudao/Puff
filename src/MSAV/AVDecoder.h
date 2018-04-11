#ifndef MSAV_AVDECODER_H
#define MSAV_AVDECODER_H

#include "MSAVGlobal.h"
#include "DPTR.h"

typedef struct AVCodecContext AVCodecContext;

namespace MSAV {

class AVDecoderPrivate;
class AVDecoder
{
    DPTR_DECLARE_PRIVATE(AVDecoder)
public:
    AVDecoder();
    virtual ~AVDecoder();

    AVCodecContext * codecCtx();

private:
    DPTR_DECLARE(AVDecoder)
};

}
#endif //MSAV_AVDECODER_H
