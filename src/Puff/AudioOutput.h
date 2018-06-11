#ifndef PUFF_AUDIO_OUTPUT_H
#define PUFF_AUDIO_OUTPUT_H

#include "AVOutput.h"
#include "AudioFormat.h"
#include <vector>

namespace Puff {

class AudioOutputPrivate;
class AudioOutput: public AVOutput
{
    DPTR_DECLARE_PRIVATE(AudioOutput)
public:
    AudioOutput();
    ~AudioOutput();

    static std::vector<std::string> backendsAvailable();

    bool open();
    bool isOpen() const;
    bool close();
    bool write(const char *data, int size, double pts);

    AudioFormat setAudioFormat(const AudioFormat& format);

    void setBackend(const std::vector<std::string> &names);

    int bufferSize() const;

    int bufferSamples() const;
    void setBufferSamples(int value);

private:

};

}
#endif //PUFF_AUDIO_OUTPUT_H
