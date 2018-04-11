#ifndef MSAV_AVCLOCK_H
#define MSAV_AVCLOCK_H

#include "MSAVGlobal.h"
#include "DPTR.h"
#include "boost/asio/deadline_timer.hpp"

namespace MSAV {

using namespace boost::asio;

class AVClockPrivate;
class AVClock
{
    DPTR_DECLARE_PRIVATE(AVClock)
public:
    AVClock();
    ~AVClock();
private:
    DPTR_DECLARE(AVClock)
};

}
#endif //MSAV_AVCLOCK_H
