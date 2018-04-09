#ifndef MSAV_AVDEMUXER_H
#define MSAV_AVDEMUXER_H

#include "MSAVGlobal.h"
#include "CObject.h"

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

    bool readFrame();
    int  videoStream();
    int  audioStream();
    int  subtitleStream();

private:

};

}
#endif //MSAV_AVDEMUXER_H
