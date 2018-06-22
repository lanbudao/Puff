#ifndef PUFF_AVDEMUXER_H
#define PUFF_AVDEMUXER_H

#include "AVGlobal.h"
#include "Packet.h"

struct AVCodecContext;

namespace Puff {

class Statistics;
class AVDemuxerPrivate;
class PU_AV_EXPORT AVDemuxer
{
    DPTR_DECLARE_PRIVATE(AVDemuxer)
public:
    AVDemuxer();
    ~AVDemuxer();

    enum StreamType {
        Stream_Audio,
        Stream_Video,
        Stream_Subtitle
    };

    void setMedia(const std::string &fileName);
    bool load();
    void unload();
    bool atEnd();

    bool isLoaded() const;

    int  stream();
    const Packet& packet() const;
    int  readFrame();
    int  videoStream();
    int  audioStream();
    int  subtitleStream();

    bool setStreamIndex(StreamType type, int index);
    bool hasAttachedPic() const;

    AVCodecContext* audioCodecCtx(int stream = -1) const;
    AVCodecContext* videoCodecCtx(int stream = -1) const;
    AVCodecContext* subtitleCodecCtx(int stream = -1) const;

    void setStatistics(Statistics *s);

    void initBaseStatistics();
    void initAudioStatistics();
    void initVideoStatistics();

    uint64_t startTime();
    uint64_t duration();

private:
    DPTR_DECLARE(AVDemuxer)
};

}
#endif //PUFF_AVDEMUXER_H
