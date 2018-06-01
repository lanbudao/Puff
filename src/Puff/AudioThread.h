#ifndef PUFF_AUDIOTHREAD_H
#define PUFF_AUDIOTHREAD_H

#include "AVThread.h"

namespace Puff {

class AudioThreadPrivate;
class PU_AV_EXPORT AudioThread: public AVThread
{
    DPTR_DECLARE_PRIVATE(AudioThread)
public:
    AudioThread();
    ~AudioThread();

protected:
    void run() PU_DECL_OVERRIDE;

private:
};

}
#endif //PUFF_AUDIOTHREAD_H
