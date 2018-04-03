#ifndef MSAV_AVDEMUXER_H
#define MSAV_AVDEMUXER_H

#include "MSAVGlobal.h"
#include "AVObject.h"

namespace MSAV {

class MS_AV_EXPORT AVDemuxer: public AVObject
{
public:
    AVDemuxer();
    ~AVDemuxer();

    void setMedia(const String &fileName);
    bool load();
    void unload();

private:
    MS_DECL_PRIVATE
};

}
#endif //MSAV_AVDEMUXER_H
