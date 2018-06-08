#include "AudioOutputBackend.h"
#include "AudioOutputBackend_p.h"

namespace Puff {

AudioOutputBackend::AudioOutputBackend():
    d_ptr(new AudioOutputBackendPrivate)
{

}

AudioOutputBackend::~AudioOutputBackend()
{

}
}
