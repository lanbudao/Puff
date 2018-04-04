#include "AVDemuxer.h"
extern "C" {
#include "libavformat/avformat.h"
}

namespace MSAV {

class AVDemuxerPrivate: public DptrPrivate<AVDemuxer>
{
public:
    AVDemuxerPrivate():
        format_ctx(NULL),
        input_format(NULL),
        format_opts(NULL)
    {

    }
    ~AVDemuxerPrivate()
    {

    }

    String fileName;
    AVFormatContext *format_ctx;
    AVInputFormat *input_format;
    AVDictionary *format_opts;
    hash<String> format_dict;
};

AVDemuxer::AVDemuxer()
{

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

    if (d.fileName.empty()) {
        return false;
    }
    if (!d.format_ctx) {
        d.format_ctx = avformat_alloc_context();
    }
    avformat_open_input(&d.format_ctx, d.fileName.data(), d.input_format, &d.format_opts);
}

void AVDemuxer::unload()
{

}

}