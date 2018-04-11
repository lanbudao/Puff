#ifndef MSAV_AVDEMUXER_H
#define MSAV_AVDEMUXER_H

#include "MSAVGlobal.h"
#include "CObject.h"
#include "Packet.h"

namespace MSAV {

class AVDemuxerPrivate;
class MS_AV_EXPORT AVDemuxer: public CObject
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
    bool readFrame();
    int  videoStream();
    int  audioStream();
    int  subtitleStream();

private:
    DPTR_DECLARE(AVDemuxer)
};

}
#endif //MSAV_AVDEMUXER_H
