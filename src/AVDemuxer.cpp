#include "AVDemuxer.h"
#include "AVError.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#ifdef __cplusplus
}
#endif

namespace MSAV {

class InterruptHandler
{
public:
    InterruptHandler(AVDemuxer *demuxer):
        mCurAction(None),
        mDemuxer(demuxer),
        interruptCB(new AVIOInterruptCB())
    {
        interruptCB->callback = handleInterrupt;
        interruptCB->opaque = this;
    }
    enum Action
    {
        None = 0,
        OpenStream,
        FindStream,
        ReadStream
    };
    ~InterruptHandler()
    {
        delete interruptCB;
    }
    AVIOInterruptCB *handler() {return interruptCB;}
    Action action() const {return mCurAction;}
    void begin(Action action)
    {

    }
    void end()
    {

    }

    void setInterruptTimeout(int64_t timeout) { mTimeout = timeout;}

    static int handleInterrupt(void *obj)
    {
        InterruptHandler *handler = static_cast<InterruptHandler *>(obj);
        if (!handler)
            return 0;
        switch (handler->action()) {
            case OpenStream:
                break;
            case FindStream:
                break;
            case ReadStream:
                break;
            default:
                break;
        }
    }

private:
    int64_t mTimeout;
    Action mCurAction;
    AVIOInterruptCB *interruptCB;
    AVDemuxer *mDemuxer;
};

class AVDemuxerPrivate: public DptrPrivate<AVDemuxer>
{
public:
    AVDemuxerPrivate():
        format_ctx(NULL),
        input_format(NULL),
        format_opts(NULL),
        interruptHandler(NULL)
    {

    }
    ~AVDemuxerPrivate()
    {
        delete interruptHandler;
        if (format_opts) {
            av_dict_free(&format_opts);
            format_opts = NULL;
        }
    }

    String fileName;
    AVFormatContext *format_ctx;
    AVInputFormat *input_format;
    AVDictionary *format_opts;
    hash<String> format_dict;
    InterruptHandler *interruptHandler;
};

AVDemuxer::AVDemuxer()
{
    DPTR_D(AVDemuxer);
    d.interruptHandler = new InterruptHandler(this);
}

AVDemuxer::~AVDemuxer()
{

}

void AVDemuxer::setMedia(const String &fileName)
{
    DPTR_D(AVDemuxer);
    d.fileName = fileName;
}

bool AVDemuxer::load()
{
    DPTR_D(AVDemuxer);
    unload();

    int ret = 0;

    if (d.fileName.empty()) {
        return false;
    }
    if (!d.format_ctx) {
        d.format_ctx = avformat_alloc_context();
    }

    /*Open Stream*/
    d.interruptHandler->begin(InterruptHandler::OpenStream);
    ret = avformat_open_input(&d.format_ctx, d.fileName.data(), d.input_format, &d.format_opts);
    d.interruptHandler->end();

    if (ret < 0) {
        AVError::ErrorCode code = AVError::OpenError;
        return false;
    }

    return true;
}

void AVDemuxer::unload()
{

}

}