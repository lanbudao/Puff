#include "AudioResample.h"
#include "AudioResample_p.h"
#include "Factory.h"
#include "commpeg.h"
#include "AVLog.h"

namespace Puff {

class AudioResampleFFmpegPrivate: public AudioResamplePrivate
{
public:
    AudioResampleFFmpegPrivate():
        context(NULL)
    {

    }
    ~AudioResampleFFmpegPrivate()
    {
        if (context) {
            swr_free(&context);
            context = NULL;
        }
    }

    SwrContext *context;
};

class AudioResampleFFmpeg: public AudioResample
{
    DPTR_DECLARE_PRIVATE(AudioResampleFFmpeg)
public:
    AudioResampleFFmpeg();
    ~AudioResampleFFmpeg();

    virtual bool prepare();
    virtual bool convert(const uchar **data);
};

extern AudioResampleId AudioResampleId_FFmpeg;
FACTORY_REGISTER(AudioResample, FFmpeg, "FFmpeg")

AudioResampleFFmpeg::AudioResampleFFmpeg():
    AudioResample(new AudioResampleFFmpegPrivate)
{

}

AudioResampleFFmpeg::~AudioResampleFFmpeg()
{

}

bool AudioResampleFFmpeg::prepare()
{
    DPTR_D(AudioResampleFFmpeg);
    if (!d->in_format.isValid()) {
        avwarnning("in format is not valid.\n");
        return false;
    }
    if (!d->in_format.channels()) {
        if (!d->in_format.channelLayoutFFmpeg()) {
            d->in_format.setChannels(2);
            d->in_format.setChannelLayoutFFmpeg(av_get_default_channel_layout(2));
        } else {
            d->in_format.setChannels(av_get_channel_layout_nb_channels(d->in_format.channelLayoutFFmpeg()));
        }
    }
    if (!d->out_format.channels()) {
        if (!d->out_format.channelLayoutFFmpeg()) {
            d->out_format.setChannels(av_get_channel_layout_nb_channels(d->out_format.channelLayoutFFmpeg()));
        } else {
            d->out_format.setChannels(d->in_format.channels());
            d->out_format.setChannelLayoutFFmpeg(av_get_default_channel_layout(d->in_format.channels()));
        }
    }
    if (!d->out_format.sampleRate()) {
        d->out_format.setSampleRate(d->in_format.sampleRate());
    }
    if (d->context)
        swr_free(&d->context);
    d->context = swr_alloc_set_opts(d->context,
                                    d->out_format.channelLayoutFFmpeg(), (AVSampleFormat)d->out_format.sampleFormatFFmpeg(), d->out_format.sampleRate(),
                                    d->in_format.channelLayoutFFmpeg(), (AVSampleFormat)d->in_format.sampleFormatFFmpeg(), d->in_format.sampleRate(),
                                    0, NULL);

    if (!d->context) {
        avwarnning("swr alloc failed!\n");
        return false;
    }
    int ret = 0;
    ret = swr_init(d->context);
    if (ret < 0) {
        avwarnning("swr init failed: %s.\n", averror2str(ret));
        swr_free(&d->context);
        d->context = NULL;
        return false;
    }
    return true;
}

bool AudioResampleFFmpeg::convert(const uchar **data)
{
    DPTR_D(AudioResampleFFmpeg);
    //Use for "a*b/c"
    d->out_samples_per_channel = av_rescale_rnd(swr_get_delay(d->context, puMax(d->in_format.sampleRate(), d->out_format.sampleRate()))
                                                    + (int64_t)d->in_samples_per_channel,
                                                (int64_t)d->out_format.sampleRate(),
                                                (int64_t)d->in_format.sampleRate(),
                                                AV_ROUND_UP);
    int sampleSize = d->out_format.channels() * d->out_format.bytesPerSample();
    int64_t outSize = d->out_samples_per_channel * sampleSize;
    if (outSize > d->data.size()) {
        d->data.resize(outSize);
    }
    uint8_t *out = (uint8_t*)d->data.data();
    int samples = swr_convert(d->context, &out, d->out_samples_per_channel,
                                          data, d->in_samples_per_channel);
    d->out_samples_per_channel = samples;
    //TODO

    return true;
}

}
