
#include "AVPlayer.h"
#include "AVDemuxer.h"

#ifdef BUILD_MSAV_LIB
#define XXXX 1
#else
#define XXXX 0
#endif

namespace MSAV {

class AVPlayerPrivate: public DptrPrivate<AVPlayer>
{
public:
    AVPlayerPrivate():
        demuxer(NULL)
    {
        demuxer = new AVDemuxer();
    }
    ~AVPlayerPrivate()
    {
        delete demuxer;
    }
    String fileName;
    hash<String> format_dict;

    /*Demuxer*/
    AVDemuxer *demuxer;
};

AVPlayer::AVPlayer()
{
    printf("AVPlayer Initialize...");
    printf("BUILD_MSAV_LIB: %d", XXXX);
}

AVPlayer::~AVPlayer()
{

}

void AVPlayer::setFile(const String &fileName)
{
    DPTR_D(AVPlayer);

    d.fileName = fileName;
}

void AVPlayer::play(const String fileName)
{
    DPTR_D(AVPlayer);

    if (fileName.empty())
        return;
    d.demuxer->setMedia(fileName);
    d.demuxer->load();
}

}