#ifndef MSAV_AVDEMUXER_H
#define MSAV_AVDEMUXER_H

#include "MSAVGlobal.h"

class MS_AV_EXPORT AVDemuxer
{
public:
    AVDemuxer();
    ~AVDemuxer();

    void setMedia(const String &fileName);
    bool load();
    void unload();

private:
    class Private;
    Private *d;
};


#endif //MSAV_AVDEMUXER_H
