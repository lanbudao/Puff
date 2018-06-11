#include "AudioOutputBackend.h"
#include "portaudio.h"
#include "AVLog.h"
#include "util.h"
#include "AudioFormat.h"
#include "Factory.h"
#include "mkid.h"

#define PA_ENSURE_OK(f, ...) PA_CHECK(f, return __VA_ARGS__;)
#define PA_CHECK(f, ...) \
    do { \
        PaError err = f; \
        if (err != paNoError) { \
            avwarnning("PortAudio error: %s, on line %d\n", Pa_GetErrorText(err), __LINE__);\
            __VA_ARGS__ \
        } \
    } while (0)

namespace Puff {

class AudioOutputPortAudio PU_NO_COPY: public AudioOutputBackend
{
public:
    AudioOutputPortAudio();
    ~AudioOutputPortAudio() PU_DECL_OVERRIDE;

    bool initialize();
    bool uninitialize();
    PaSampleFormat toPaSampleFormat(const AudioFormat::SampleFormat &format);

    bool open() PU_DECL_OVERRIDE;
    bool close() PU_DECL_OVERRIDE;
    bool write(const char *data, int size) PU_DECL_OVERRIDE;

private:
    PaStreamParameters *stream_paras;
    PaStream *stream;
    bool isInitialized;
    double outputLatency;
};

typedef AudioOutputPortAudio AudioOutputBackendPortAudio;
static const AudioOutputBackendId AudioOutputBackendId_PortAudio = mkid::id32base36_6<'P', 'o', 'r', 't', 'A', 'u'>::value;
FACTORY_REGISTER(AudioOutputBackend, PortAudio, "PortAudio")

AudioOutputPortAudio::AudioOutputPortAudio():
    stream_paras(NULL),
    isInitialized(false),
    outputLatency(0.0),
    stream(NULL)
{
    avdebug("PortAudio' version %d, %s", Pa_GetVersion(), Pa_GetVersionText());

    if (!initialize()) {
        return;
    }

    int devNum = Pa_GetDeviceCount();
    for (int i = 0; i < devNum; ++i) {
        const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(i);
        if (!devInfo)
            continue;
        const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(devInfo->hostApi);
        if (!hostApiInfo)
            continue;
        std::string name = sformat("%s: %s", devInfo->name, hostApiInfo->name);
        avdebug("audio device %d: %s\n", i, name.c_str());
        avdebug("max in/out channel: %d / %d\n", devInfo->maxInputChannels, devInfo->maxOutputChannels);
    }

    /*init stream paras*/
    stream_paras = new PaStreamParameters;
    memset(stream_paras, 0, sizeof(PaStreamParameters));
    stream_paras->device = Pa_GetDefaultOutputDevice();
    if (stream_paras->device == paNoDevice) {
        avwarnning("PortAudio get default device error!");
        return;
    }
    const PaDeviceInfo *info = Pa_GetDeviceInfo(stream_paras->device);
    if (!info)
        return;
    avdebug("default audio device: %s\n", info->name);
    avdebug("default max in/out channel: %d / %d\n", info->maxInputChannels, info->maxOutputChannels);
    stream_paras->hostApiSpecificStreamInfo = NULL;
    stream_paras->suggestedLatency = info->defaultHighOutputLatency;
}

AudioOutputPortAudio::~AudioOutputPortAudio()
{
    if (stream_paras) {
        delete stream_paras;
        stream_paras = NULL;
    }
}

bool AudioOutputPortAudio::initialize()
{
    if (isInitialized)
        return true;
    PA_ENSURE_OK(Pa_Initialize(), false);
    isInitialized = true;
    return true;
}

bool AudioOutputPortAudio::uninitialize()
{
    if (!isInitialized)
        return true;
    PA_ENSURE_OK(Pa_Terminate(), false);
    isInitialized = false;
    return true;
}

PaSampleFormat AudioOutputPortAudio::toPaSampleFormat(const AudioFormat::SampleFormat &format)
{
    switch (format) {
    case AudioFormat::SampleFormat_Unsigned8:
        return paUInt8;
    case AudioFormat::SampleFormat_Signed8:
        return paInt8;
    case AudioFormat::SampleFormat_Signed16:
        return paInt16;
    case AudioFormat::SampleFormat_Signed24:
        return paInt24;
    case AudioFormat::SampleFormat_Signed32:
        return paInt32;
    case AudioFormat::SampleFormat_Float:
        return paFloat32;
    default:
        return paCustomFormat;
    }
}

bool AudioOutputPortAudio::open()
{
    if (!isInitialized) {
        if (!initialize()) {
            return false;
        }
    }
    stream_paras->sampleFormat = toPaSampleFormat(format()->sampleFormat());
    stream_paras->channelCount = format()->channels();
    /*Don't use callback function*/
    PA_ENSURE_OK(Pa_OpenStream(&stream, NULL, stream_paras, format()->sampleRate(), 0, paNoFlag, NULL, NULL), false);
    outputLatency = Pa_GetStreamInfo(stream)->outputLatency;
    return true;
}

bool AudioOutputPortAudio::close()
{
    if (!stream) {
        return uninitialize();
    }
    PA_ENSURE_OK(Pa_StopStream(stream), false);
    PA_ENSURE_OK(Pa_CloseStream(stream), false);
    stream = NULL;
    return uninitialize();
}

bool AudioOutputPortAudio::write(const char *data, int size)
{
    if (!stream || !stream_paras)
        return false;
    if (Pa_IsStreamStopped(stream))
        Pa_StartStream(stream);
    PaError err = Pa_WriteStream(stream, data, size / stream_paras->channelCount / format()->bytesPerSample());
    if (err == paUnanticipatedHostError) {
        avwarnning("PortAudio write stream error: %s\n", Pa_GetErrorText(err));
        return false;
    }
    return true;
}

}
