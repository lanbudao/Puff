#include "AudioOutputBackend.h"
#include "portaudio.h"
#include "AVLog.h"

namespace Puff {

class AudioOutputPortAudio PU_NO_COPY: public AudioOutputBackend
{
public:
    AudioOutputPortAudio()
    {
        avdebug("PortAudio' version %d, %s", Pa_GetVersion(), Pa_GetVersionText());
    }
    ~AudioOutputPortAudio() PU_DECL_OVERRIDE
    {

    }

    bool open() PU_DECL_OVERRIDE;
    bool close() PU_DECL_OVERRIDE;
    bool clear() PU_DECL_OVERRIDE;

private:

};

bool AudioOutputPortAudio::open()
{

    return true;
}

bool AudioOutputPortAudio::close()
{

    return true;
}

bool AudioOutputPortAudio::clear()
{

    return true;
}

}
