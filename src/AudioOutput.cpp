#include "AudioOutput.h"
#include "AVOutput_p.h"
#include "AudioOutputBackend.h"
#include "AVLog.h"

namespace Puff {

static const int kBufferSamples = 512;
static const int kBufferCount = 8*2;

class AudioOutputPrivate: public AVOutputPrivate
{
public:
    AudioOutputPrivate():
        backend(NULL),
        available(false),
        buffer_samples(kBufferSamples)
    {

    }
    ~AudioOutputPrivate()
    {

    }

    AudioFormat format;
    std::vector<std::string> backendNames;
    AudioOutputBackend *backend;
    bool available;
    int buffer_samples;
};

AudioOutput::AudioOutput():
    AVOutput(new AudioOutputPrivate)
{
    std::vector<std::string> backends = backendsAvailable();
    for (int i = 0; i < backends.size(); ++i) {
        avdebug("audio output backend: ", backends.at(i));
    }
    setBackend(AudioOutputBackend::defaultPriority());
}

AudioOutput::~AudioOutput()
{

}

extern void AudioOutput_RegisterAll();
std::vector<string> AudioOutput::backendsAvailable()
{
    AudioOutput_RegisterAll();
    static std::vector<string> backends;
    if (!backends.empty())
        return backends;
    AudioOutputBackendId *i = NULL;
    while ((i = AudioOutputBackend::next(i)) != NULL) {
        backends.push_back(AudioOutputBackend::name(*i));
    }
    return backends;
}

bool AudioOutput::open()
{
    DPTR_D(AudioOutput);
    if (!d->backend)
        return false;
    if (!d->backend->open())
        return false;
    d->available = true;
    return true;
}

bool AudioOutput::isOpen() const
{
    return d_func()->available;
}

bool AudioOutput::close()
{
    DPTR_D(AudioOutput);
    if (!d->backend)
        return false;
    if (!d->backend->close())
        return false;
    d->available = false;
    return true;
}

bool AudioOutput::write(const char *data, int size, double pts)
{
    DPTR_D(AudioOutput);
    if (!d->backend)
        return false;
    d->backend->write(data, size);
    return true;
}

AudioFormat AudioOutput::setAudioFormat(const AudioFormat &format)
{
    DPTR_D(AudioOutput);
    if (d->format == format)
        return format;
    d->format = format;
    d->backend->setFormat(format);
    return format;
}

void AudioOutput::setBackend(const std::vector<std::string> &names)
{
    DPTR_D(AudioOutput);
    d->backendNames = names;

    if (d->backend) {
        d->backend->close();
        delete d->backend;
        d->backend = NULL;
    }
    int size = d->backendNames.size();
    for (int i = 0; i < size; ++i) {
        d->backend = AudioOutputBackend::create(d->backendNames.at(i).c_str());
        if (!d->backend)
            continue;
        if (d->backend->avaliable())
            break;
        delete d->backend;
        d->backend = NULL;
    }
    if (d->backend) {

    }
}

int AudioOutput::bufferSize() const
{
    DPTR_D(const AudioOutput);
    return bufferSamples() * d->format.bytesPerSample();
}

int AudioOutput::bufferSamples() const
{
    DPTR_D(const AudioOutput);
    return d->buffer_samples;
}

void AudioOutput::setBufferSamples(int value)
{
    DPTR_D(AudioOutput);
    d->buffer_samples = value;
}

}
