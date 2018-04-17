#ifndef PUFF_AVDECODER_P_H
#define PUFF_AVDECODER_P_H

#include "AVGlobal.h"
#include "DPTR.h"
#include "commpeg.h"

namespace Puff {

class AVDecoderPrivate: public DptrPrivate<AVDecoder>
{
public:
    AVDecoderPrivate():
        codec_ctx(NULL),
        dict(NULL),
        opened(false)
    {
        codec_ctx = avcodec_alloc_context3(NULL);
    }
    ~AVDecoderPrivate()
    {
        if (dict)
            av_dict_free(&dict);
        avcodec_free_context(&codec_ctx);
    }

    /*Internal Open/Close*/
    virtual bool open() {return true;}
    virtual bool close() {return true;}

    void flush();
    void applyOptionsForDict();

    bool opened;
    AVCodecContext *codec_ctx;
    String codec_name;
    String hwaccel;
    std::hash<String> options;
    AVDictionary *dict;
};

class VideoDecoderPrivate: public AVDecoderPrivate
{
public:
    VideoDecoderPrivate() {}
    ~VideoDecoderPrivate() {}
};

}
#endif //PUFF_AVDECODER_P_H
