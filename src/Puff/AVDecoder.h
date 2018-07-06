#ifndef PUFF_AVDECODER_H
#define PUFF_AVDECODER_H

#include "CObject.h"

typedef struct AVCodecContext AVCodecContext;
typedef struct AVCodec AVCodec;
namespace Puff {

class AVDecoderPrivate;
class PU_AV_EXPORT AVDecoder: public CObject
{
    DPTR_DECLARE_PRIVATE(AVDecoder)
public:
    virtual ~AVDecoder();

    AVCodecContext * codecCtx();

    virtual std::string name() const = 0;
    virtual std::string description() const = 0;

    virtual bool open(const string &extra = string());
    virtual bool close();

    bool isOpen();
    void flush();

    void setCodecName(const std::string &name);
    std::string codecName() const;

    bool isAvailable() const;

    void setCodecCtx(void *ctx);
    AVCodec *findCodec(const std::string &name, const std::string &hwaccel, int id);

protected:
    AVDecoder(AVDecoderPrivate *d);
    DPTR_DECLARE(AVDecoder)
};

}
#endif //PUFF_AVDECODER_H
