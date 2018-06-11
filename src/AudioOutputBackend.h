#ifndef PUFF_AUDIO_OUTPUT_BACKEND_H
#define PUFF_AUDIO_OUTPUT_BACKEND_H

#include "CObject.h"

typedef int AudioOutputBackendId;

namespace Puff {

class AudioFormat;
class AudioOutputBackendPrivate;
class AudioOutputBackend: public CObject
{
    DPTR_DECLARE_PRIVATE(AudioOutputBackend)
public:
    AudioOutputBackend();
    virtual ~AudioOutputBackend();

    static std::vector<std::string> defaultPriority();
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool clear() {return false;}
    virtual bool write(const char *data, int size) = 0;

    bool avaliable() const;
    void setAvaliable(bool b);
    void setFormat(const AudioFormat &fmt);

public:
    template<class C> static bool Register(AudioOutputBackendId id, const char* name) { return Register(id, create<C>, name);}
    static AudioOutputBackend* create(AudioOutputBackendId id);
    static AudioOutputBackend* create(const char* name);
    static AudioOutputBackendId* next(AudioOutputBackendId* id = 0);
    static const char* name(AudioOutputBackendId id);
    static AudioOutputBackendId id(const char* name);

private:
    template<class C> static AudioOutputBackend* create() { return new C();}
    typedef AudioOutputBackend* (*AudioOutputBackendCreator)();
    static bool Register(AudioOutputBackendId id, AudioOutputBackendCreator, const char *name);

protected:
    const AudioFormat *format();

protected:
    DPTR_DECLARE(AudioOutputBackend)
};

}
#endif //PUFF_AUDIO_OUTPUT_BACKEND_H
