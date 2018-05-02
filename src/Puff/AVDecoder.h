#ifndef PUFF_AVDECODER_H
#define PUFF_AVDECODER_H

#include "AVGlobal.h"
#include "DPTR.h"

typedef struct AVCodecContext AVCodecContext;

namespace Puff {

class AVDecoderPrivate;
class PU_AV_EXPORT AVDecoder
{
    DPTR_DECLARE_PRIVATE(AVDecoder)
public:
    AVDecoder();
    virtual ~AVDecoder();

    AVCodecContext * codecCtx();

    virtual String name() const = 0;
    virtual String description() const = 0;

    virtual bool open();
    virtual bool close();

    bool isOpen();
    void flush();

    void setCodecName(const String &name);
    String codecName() const;

    bool isAvailable() const;

protected:
    DPTR_DECLARE(AVDecoder)
};

}
#endif //PUFF_AVDECODER_H
