#ifndef PUFF_AVOUTPUT_P_H
#define PUFF_AVOUTPUT_P_H

#include "DPTR.h"
#include "size.h"
#include "Statistics.h"

namespace Puff {

class AVOutputPrivate
{
public:
    AVOutputPrivate():
        avaliable(true),
        pause(false),
        statistics(NULL)
    {

    }
    virtual ~AVOutputPrivate()
    {

    }

    bool avaliable;
    bool pause;
    Statistics *statistics;
};

}

#endif //PUFF_AVOUTPUT_P_H
