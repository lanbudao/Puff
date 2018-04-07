//
// Created by giga on 2018/4/4.
//

#include "AVClock.h"

namespace MSAV {

class AVClockPrivate: public DptrPrivate<AVClock>
{
public:
    AVClockPrivate():
        timer(NULL)
    {

    }
    ~AVClockPrivate()
    {
        delete timer;
    }
    deadline_timer *timer;
};

AVClock::AVClock() {

}

AVClock::~AVClock() {

}
}