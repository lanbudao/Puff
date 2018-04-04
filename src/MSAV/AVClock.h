#ifndef MSAV_AVCLOCK_H
#define MSAV_AVCLOCK_H

#include "MSAVGlobal.h"
#include "DPTR.h"

namespace MSAV {

class AVClockPrivate;
class AVClock
{
    DPTR_DECLARE_PRIVATE(AVClock)
public:
    AVClock();
    ~AVClock();
};

}
#endif //MSAV_AVCLOCK_H
