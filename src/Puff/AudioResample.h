#ifndef PUFF_AUDIO_RESAMPLE_H
#define PUFF_AUDIO_RESAMPLE_H

#include "CObject.h"
#include "ByteArray.h"

typedef int AudioResampleId;

namespace Puff {

class AudioFormat;
class AudioResamplePrivate;
class PU_AV_EXPORT AudioResample: public CObject
{
    DPTR_DECLARE_PRIVATE(AudioResample)
public:
    virtual ~AudioResample();

    const ByteArray& outData() const;

    virtual bool prepare() = 0;
    virtual bool convert(const uchar **data) = 0;

    void setInFormat(const AudioFormat &fmt);
    void setOutFormat(const AudioFormat &fmt);
    void setOutSampleFormat(int sample_fmt);
    void setInSamplesPerChannel(int spc);

    int outSamplesPerChannel() const;

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
    AudioResample(AudioResamplePrivate *d);
    DPTR_DECLARE(AudioResample)
};

extern PU_AV_EXPORT AudioResampleId AudioResampleId_FFmpeg;

}
#endif //PUFF_AUDIO_RESAMPLE_H
