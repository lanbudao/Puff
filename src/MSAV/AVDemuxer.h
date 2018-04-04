#ifndef MSAV_AVDEMUXER_H
#define MSAV_AVDEMUXER_H

#include "MSAVGlobal.h"
#include "AVObject.h"

namespace MSAV {

class AVDemuxerPrivate;
class MS_AV_EXPORT AVDemuxer: public AVObject
{
    DPTR_DECLARE_PRIVATE(AVDemuxer);
public:
    AVDemuxer();
    ~AVDemuxer();

    void setMedia(const String &fileName);
    bool load();
    void unload();

private:

};

}
#endif //MSAV_AVDEMUXER_H
