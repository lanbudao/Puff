#include "AudioResample.h"
#include "AudioResample_p.h"
#include "mkid.h"
#include "Factory.h"

namespace Puff {

class AudioResampleFFmpegPrivate: public AudioResamplePrivate
{
public:
    AudioResampleFFmpegPrivate()
    {

    }
    ~AudioResampleFFmpegPrivate()
    {

    }
};

class AudioResampleFFmpeg: public AudioResample
{
public:
    AudioResampleFFmpeg();
    ~AudioResampleFFmpeg();
};

static const AudioResampleId AudioResampleId_FFmpeg = mkid::id32base36_6<'F', 'F', 'm', 'p', 'e', 'g'>::value;
FACTORY_REGISTER(AudioResample, FFmpeg, "FFmpeg")

AudioResampleFFmpeg::AudioResampleFFmpeg():
    d_ptr(new AudioResampleFFmpegPrivate)
{

}

AudioResampleFFmpeg::~AudioResampleFFmpeg()
{

}

}
