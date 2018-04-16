#ifndef PUFF_AVCLOCK_H
#define PUFF_AVCLOCK_H

#include "AVGlobal.h"
#include "DPTR.h"
#include "boost/asio/deadline_timer.hpp"

namespace Puff {

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
#endif //PUFF_AVCLOCK_H
