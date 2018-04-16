#ifndef PUFF_AVDECODER_H
#define PUFF_AVDECODER_H

#include "AVGlobal.h"
#include "DPTR.h"

typedef struct AVCodecContext AVCodecContext;

namespace Puff {

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
#endif //PUFF_AVDECODER_H
