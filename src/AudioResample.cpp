#include "AudioResample.h"
#include "AudioResample_p.h"
#include "Factory.h"
#include "mkid.h"

namespace Puff {

FACTORY_DEFINE(AudioResample)

AudioResampleId AudioResampleId_FFmpeg = mkid::id32base36_6<'F', 'F', 'm', 'p', 'e', 'g'>::value;
void AudioResample_RegisterAll()
{
    ONLY_RUN_ONES;
    if (!AudioResampleFactory::instance().registeredIds().empty()) {
        return;
    }
    extern bool RegisterAudioResampleFFmpeg_Man();
    RegisterAudioResampleFFmpeg_Man();
}

AudioResample::AudioResample(AudioResamplePrivate *d):
    d_ptr(d)
{

}

AudioResample::~AudioResample()
{

}

const ByteArray& AudioResample::outData() const
{
    DPTR_D(const AudioResample);
    return d->data;
}

void AudioResample::setInFormat(const AudioFormat &fmt)
{
    DPTR_D(AudioResample);
    d->in_format = fmt;
}

void AudioResample::setOutFormat(const AudioFormat &fmt)
{
    DPTR_D(AudioResample);
    d->out_format = fmt;
    prepare();
}

void AudioResample::setOutSampleFormat(int sample_fmt)
{
    DPTR_D(AudioResample);
    AudioFormat fmt(d->out_format);
    fmt.setSampleFormatFFmpeg(sample_fmt);
    setOutFormat(fmt);
}

void AudioResample::setInSamplesPerChannel(int spc)
{
    DPTR_D(AudioResample);
    d->in_samples_per_channel = spc;
}

int AudioResample::outSamplesPerChannel() const
{
    DPTR_D(const AudioResample);
    return d->out_samples_per_channel;
}

}
