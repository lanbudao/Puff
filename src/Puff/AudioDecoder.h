#ifndef PUFF_AUDIODECODER_H
#define PUFF_AUDIODECODER_H

#include "AVDecoder.h"

namespace Puff {

class AudioDecoderPrivate;
class AudioDecoder: public AVDecoder
{
    DPTR_DECLARE_PRIVATE(AudioDecoder)
public:
    AudioDecoder();
    ~AudioDecoder();

    static StringList supportedCodecs();

private:

};

}
#endif //PUFF_AUDIODECODER_H
