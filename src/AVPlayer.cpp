
#include "AVPlayer.h"
#include "AVDemuxer.h"
#include "AVDemuxThread.h"

namespace MSAV {

class AVPlayerPrivate: public DptrPrivate<AVPlayer>
{
public:
    AVPlayerPrivate():
        demuxer(NULL),
        demux_thread(NULL)
    {
        demuxer = new AVDemuxer();
        demux_thread = new AVDemuxThread();
        demux_thread->setDemuxer(demuxer);
    }
    ~AVPlayerPrivate()
    {
        delete demuxer;
    }
    String fileName;
    std::hash<String> format_dict;

    /*Demuxer*/
    AVDemuxer *demuxer;
    AVDemuxThread *demux_thread;
};

AVPlayer::AVPlayer()
{
    printf("AVPlayer Initialize...\n");
}

AVPlayer::~AVPlayer()
{

}

void AVPlayer::setFile(const String &fileName)
{
    DPTR_D(AVPlayer);

    d.fileName = fileName;
}

void AVPlayer::play(const String& fileName)
{
    DPTR_D(AVPlayer);

    if (fileName.empty())
        return;
    d.demuxer->setMedia(fileName);
    d.demuxer->load();
}

}