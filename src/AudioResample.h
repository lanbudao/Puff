#ifndef PUFF_AUDIO_RESAMPLE_H
#define PUFF_AUDIO_RESAMPLE_H

#include "CObject.h"

typedef int AudioResampleId;

namespace Puff {

class AudioFormat;
class AudioResamplePrivate;
class AudioResample: public CObject
{
    DPTR_DECLARE_PRIVATE(AudioResample)
public:
    AudioResample();
    virtual ~AudioResample();

public:
    template<class C> static bool Register(AudioResampleId id, const char* name) { return Register(id, create<C>, name);}
    static AudioResample* create(AudioResampleId id);
    static AudioResample* create(const char* name);
    static AudioResampleId* next(AudioResampleId* id = 0);
    static const char* name(AudioResampleId id);
    static AudioResampleId id(const char* name);

private:
    template<class C> static AudioResample* create() { return new C();}
    typedef AudioResample* (*AudioResampleCreator)();
    static bool Register(AudioResampleId id, AudioResampleCreator, const char *name);

protected:

protected:
    DPTR_DECLARE(AudioResample)
};

}
#endif //PUFF_AUDIO_RESAMPLE_H
