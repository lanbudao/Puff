#ifndef MSAV_AUDIOTHREAD_H
#define MSAV_AUDIOTHREAD_H

#include "AVThread.h"

namespace MSAV {

class AudioThreadPrivate;
class MS_AV_EXPORT AudioThread: public AVThread
{
    DPTR_DECLARE_PRIVATE(AudioThread)
public:
    AudioThread();
    ~AudioThread();

protected:
    void run() MS_DECL_OVERRIDE;

private:
    DPTR_DECLARE(AudioThread)
};

}
#endif //MSAV_AUDIOTHREAD_H
