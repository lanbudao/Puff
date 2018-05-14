#ifndef PUFF_AUDIODECODER_H
#define PUFF_AUDIODECODER_H

#include "AVDecoder.h"

namespace Puff {

class AudioDecoderPrivate;
class PU_AV_EXPORT AudioDecoder: public AVDecoder
{
    DPTR_DECLARE_PRIVATE(AudioDecoder)
public:
    AudioDecoder();
    ~AudioDecoder();

    static StringList supportedCodecs();

    virtual std::string name() const;
    virtual std::string description() const;

private:

};

}
#endif //PUFF_AUDIODECODER_H
