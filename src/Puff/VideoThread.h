#ifndef PUFF_VIDEOTHREAD_H
#define PUFF_VIDEOTHREAD_H

#include "AVThread.h"

namespace Puff {

class VideoThreadPrivate;
class VideoThread: public AVThread
{
    DPTR_DECLARE_PRIVATE(VideoThread)
public:
    VideoThread();
    ~VideoThread();

protected:
    void run() PU_DECL_OVERRIDE;
};
}
#endif //PUFF_VIDEOTHREAD_H
