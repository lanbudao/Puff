#include "AudioFormat.h"
#include "commpeg.h"

namespace Puff {

static const struct {
    int64_t ff;
    AudioFormat::ChannelLayout cl;
} channelLayoutMap[] = {
        { AV_CH_FRONT_LEFT, AudioFormat::ChannelLayout_Left },
        { AV_CH_FRONT_RIGHT, AudioFormat::ChannelLayout_Right },
        { AV_CH_FRONT_CENTER, AudioFormat::ChannelLayout_Center },
        { AV_CH_LAYOUT_MONO, AudioFormat::ChannelLayout_Mono },
        { AV_CH_LAYOUT_STEREO, AudioFormat::ChannelLayout_Stereo },
        { 0, AudioFormat::ChannelLayout_Unsupported}
};

class AudioFormatPrivate: public DptrPrivate<AudioFormat>
{
public:
    AudioFormatPrivate():
        sample_fmt(AudioFormat::SampleFormat_Unknown),
        av_sample_fmt(AV_SAMPLE_FMT_NONE),
        sample_rate(0),
        channels(0)
    {

    }
    ~AudioFormatPrivate()
    {

    }

    void setChannels(int cls)
    {
        channels = cls;
        if (av_get_channel_layout_nb_channels(channel_layout_ff) != channels) {
            channel_layout_ff = av_get_default_channel_layout(channels);
            channel_layout = AudioFormat::channelLayoutFromFFmpeg(channel_layout_ff);
        }
    }

    void setChannelLayoutFFmpeg(int64_t clff)
    {
        channel_layout_ff = clff;
        channels = av_get_channel_layout_nb_channels(channel_layout_ff);
    }

    AudioFormat::SampleFormat sample_fmt;
    AVSampleFormat av_sample_fmt;
    AudioFormat::ChannelLayout channel_layout;
    int sample_rate;
    int channels;
    int64_t channel_layout_ff;
};

AudioFormat::AudioFormat() {

}

AudioFormat::~AudioFormat() {

}

AudioFormat::ChannelLayout AudioFormat::channelLayoutFromFFmpeg(int64_t clff) {
    for (int i = 0; i < sizeof(channelLayoutMap) / sizeof(channelLayoutMap[0]); ++i) {
        if (channelLayoutMap[i].ff == clff)
            return channelLayoutMap[i].cl;
    }
    return ChannelLayout_Unsupported;
}

int64_t AudioFormat::channelLayoutToFFmpeg(AudioFormat::ChannelLayout cl) {
    for (int i = 0; i < sizeof(channelLayoutMap) / sizeof(channelLayoutMap[0]); ++i) {
        if (channelLayoutMap[i].cl == cl)
            return channelLayoutMap[i].ff;
    }
    return 0;
}

bool AudioFormat::isValid() const {
    DPTR_D(AudioFormat);
    return d.sample_fmt > 0 &&
            (d.channels > 0 || d.channel_layout > 0) &&
            d.sample_fmt != AudioFormat::SampleFormat_Unknown;
}

bool AudioFormat::isFloat() const {
    DPTR_D(AudioFormat);
    return !!(d.sample_fmt & kFloat);
}

bool AudioFormat::isPlanar() const {
    DPTR_D(AudioFormat);
    return !!(d.sample_fmt & kPlanar);
}

bool AudioFormat::isUnsigned() const {
    DPTR_D(AudioFormat);
    return !!(d.sample_fmt & kUnsigned);
}

int AudioFormat::channels() const {
    DPTR_D(const AudioFormat);
    return d.channels;
}

void AudioFormat::setChannels(int cls) {
    DPTR_D(AudioFormat);
    d.setChannels(cls);
}

int AudioFormat::bytesPerSample() const {
    DPTR_D(const AudioFormat);
    return d.sample_fmt & (((1 << (kSize + 1)) - 1));
}

int AudioFormat::bytesPerFrame() const {
    if (!isValid())
        return 0;
    return bytesPerSample() * channels();
}

int64_t AudioFormat::durationForBytes(int64_t bytes) {
    DPTR_D(const AudioFormat);
    if (!isValid() || bytes <= 0)
        return 0;
    return int64_t(kHz * (bytes / bytesPerFrame())) / sampleRate();
}

int AudioFormat::sampleRate() const {
    DPTR_D(const AudioFormat);
    return d.sample_rate;
}

void AudioFormat::setSampleRate(int rate) {
    DPTR_D(AudioFormat);
    d.sample_rate = rate;
}

int AudioFormat::planeCount() const {
    return isPlanar() ? channels() : 1;
}

}
