#ifndef PUFF_AUDIO_OUTPUT_BACKEND_H
#define PUFF_AUDIO_OUTPUT_BACKEND_H

#include "CObject.h"

namespace Puff {

class AudioOutputBackendPrivate;
class AudioOutputBackend: public CObject
{
    DPTR_DECLARE_PRIVATE(AudioOutputBackend)
public:
    AudioOutputBackend();
    virtual ~AudioOutputBackend();

    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool clear() = 0;

protected:
    DPTR_DECLARE(AudioOutputBackend)
};

}
#endif //PUFF_AUDIO_OUTPUT_BACKEND_H
