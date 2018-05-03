#ifndef PUFF_AVOUTPUT_H
#define PUFF_AVOUTPUT_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class AVOutputPrivate;
class PU_AV_EXPORT AVOutput
{
    DPTR_DECLARE_PRIVATE(AVOutput)
public:
    AVOutput();
    virtual ~AVOutput();

    bool isAvaliable() const;
    void pause(bool p);
    bool isPaused() const;

private:
    DPTR_DECLARE(AVOutput)
};

}
#endif //PUFF_AVOUTPUT_H
