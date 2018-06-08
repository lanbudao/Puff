#ifndef PUFF_AUDIO_OUTPUT_H
#define PUFF_AUDIO_OUTPUT_H

#include "AVOutput.h"
#include "AudioFormat.h"

namespace Puff {

class AudioOutputPrivate;
class AudioOutput: public AVOutput
{
    DPTR_DECLARE_PRIVATE(AudioOutput)
public:
    AudioOutput();
    ~AudioOutput();

    bool open();
    bool close();

    AudioFormat setAudioFormat(const AudioFormat& format);

private:

};

}
#endif //PUFF_AUDIO_OUTPUT_H
