#ifndef MSAV_AUDIOTHREAD_H
#define MSAV_AUDIOTHREAD_H

#include "AVThread.h"

namespace MSAV {

class MS_AV_EXPORT AudioThread: public AVThread
{
public:
    AudioThread();
    ~AudioThread();

protected:
    void run() MS_DECL_OVERRIDE;
};

}
#endif //MSAV_AUDIOTHREAD_H
