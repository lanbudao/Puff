#ifndef PUFF_AUDIO_OUTPUT_BACKEND_P_H
#define PUFF_AUDIO_OUTPUT_BACKEND_P_H

#include "AudioFormat.h"

namespace Puff {

class AudioOutputBackendPrivate
{
public:
    AudioOutputBackendPrivate():
        avaliable(false)
    {

    }
    virtual ~AudioOutputBackendPrivate()
    {

    }

    AudioFormat format;
    bool avaliable;
};

}
#endif //PUFF_AUDIO_OUTPUT_BACKEND_P_H
