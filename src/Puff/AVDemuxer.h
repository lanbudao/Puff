#ifndef PUFF_AVDEMUXER_H
#define PUFF_AVDEMUXER_H

#include "AVGlobal.h"
#include "CObject.h"
#include "Packet.h"

namespace Puff {

class AVDemuxerPrivate;
class PU_AV_EXPORT AVDemuxer: public CObject
{
    DPTR_DECLARE_PRIVATE(AVDemuxer);
public:
    AVDemuxer();
    ~AVDemuxer();

    void setMedia(const String &fileName);
    bool load();
    void unload();

    int  stream();
    Packet packet() const;
    int  readFrame();
    int  videoStream();
    int  audioStream();
    int  subtitleStream();

private:
    DPTR_DECLARE(AVDemuxer)
};

}
#endif //PUFF_AVDEMUXER_H
