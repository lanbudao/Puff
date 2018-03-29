#ifndef MSAV_AVDEMUXER_H
#define MSAV_AVDEMUXER_H

#include "MSAVGlobal.h"

class AVDemuxer
{
public:
    AVDemuxer();
    ~AVDemuxer();

    void setMedia(const String &fileName);
    bool load();
    void unload();

private:
    class AVDemuxerPrivate;
    AVDemuxerPrivate *d;
};


#endif //MSAV_AVDEMUXER_H
