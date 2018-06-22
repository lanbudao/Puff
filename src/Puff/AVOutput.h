#ifndef PUFF_AVOUTPUT_H
#define PUFF_AVOUTPUT_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class Statistics;
class AVOutputPrivate;
class PU_AV_EXPORT AVOutput
{
    DPTR_DECLARE_PRIVATE(AVOutput)
public:
    virtual ~AVOutput();

    bool isAvaliable() const;
    void setAvaliable(bool avaliable);
    void pause(bool p);
    bool isPaused() const;

private:
    virtual void setStatistics(Statistics *s);
    friend class AVPlayer;

protected:
    AVOutput(AVOutputPrivate *d);
    DPTR_DECLARE(AVOutput)

};

}
#endif //PUFF_AVOUTPUT_H
