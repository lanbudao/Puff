#ifndef PUFF_AUDIO_RESAMPLE_P_H
#define PUFF_AUDIO_RESAMPLE_P_H

#include "AudioFormat.h"
#include "commpeg.h"

namespace Puff {

class AudioResamplePrivate
{
public:
    AudioResamplePrivate():
        context(NULL)
    {

    }
    virtual ~AudioResamplePrivate()
    {
        if (context) {
            swr_free(&context);
            context = NULL;
        }
    }

    SwrContext *context;
};

}
#endif //PUFF_AUDIO_RESAMPLE_P_H
