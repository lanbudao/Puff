#ifndef PUFF_AUDIO_RESAMPLE_P_H
#define PUFF_AUDIO_RESAMPLE_P_H

#include "AudioFormat.h"

namespace Puff {

class AudioResamplePrivate
{
public:
    AudioResamplePrivate():
        in_samples_per_channel(0),
        out_samples_per_channel(0)
    {
        in_format.setSampleFormat(AudioFormat::SampleFormat_Unknown);
        out_format.setSampleFormat(AudioFormat::SampleFormat_Float);
    }
    virtual ~AudioResamplePrivate()
    {
    }

    int64_t in_samples_per_channel, out_samples_per_channel;
    AudioFormat in_format, out_format;

    ByteArray data;
};

}
#endif //PUFF_AUDIO_RESAMPLE_P_H
