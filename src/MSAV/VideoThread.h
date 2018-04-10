#ifndef MSAV_VIDEOTHREAD_H
#define MSAV_VIDEOTHREAD_H

#include "AVThread.h"

namespace MSAV {

class VideoThread: public AVThread
{
public:
    VideoThread();
    ~VideoThread();

protected:
    void run() MS_DECL_OVERRIDE;
};
}
#endif //MSAV_VIDEOTHREAD_H
