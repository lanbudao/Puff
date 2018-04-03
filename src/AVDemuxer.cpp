#include "AVDemuxer.h"
extern "C" {
#include "libavformat/avformat.h"
}

namespace MSAV {

class AVDemuxer::Private
{
public:
    Private():
        format_ctx(NULL),
        input_format(NULL),
        format_opts(NULL)
    {

    }
    ~Private()
    {

    }

    String fileName;
    AVFormatContext *format_ctx;
    AVInputFormat *input_format;
    AVDictionary *format_opts;
    hash<String> format_dict;
};

AVDemuxer::AVDemuxer():
    d(new Private())
{

}

AVDemuxer::~AVDemuxer()
{
    delete d;
}

void AVDemuxer::setMedia(const String &fileName)
{
    d->fileName = fileName;
}

bool AVDemuxer::load()
{
    unload();

    if (d->fileName.empty()) {
        return false;
    }
    if (!d->format_ctx) {
        d->format_ctx = avformat_alloc_context();
    }
    avformat_open_input(&d->format_ctx, d->fileName.data(), d->input_format, &d->format_opts);
}

void AVDemuxer::unload()
{

}

}