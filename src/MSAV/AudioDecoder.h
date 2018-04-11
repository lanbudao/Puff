#ifndef MSAV_AUDIODECODER_H
#define MSAV_AUDIODECODER_H

#include "AVDecoder.h"

namespace MSAV {

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
#endif //MSAV_AUDIODECODER_H
