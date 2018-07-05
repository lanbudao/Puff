#ifndef PUFF_FILTER_H
#define PUFF_FILTER_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class AVPlayer;
class FilterPrivate;
class PU_AV_EXPORT Filter
{
    DPTR_DECLARE_PRIVATE(Filter)
public:
    virtual ~Filter();

    bool enabled() const;
    void setEnabled(bool b);

protected:
    Filter(FilterPrivate *d);
    DPTR_DECLARE(Filter)
};

class AudioFilterPrivate;
class AudioFilter: public Filter
{
    DPTR_DECLARE_PRIVATE(Filter)
public:
    AudioFilter();
    virtual ~AudioFilter();

    void installTo(AVPlayer *player);

protected:
    AudioFilter(AudioFilterPrivate *d);
};

class VideoFilterPrivate;
class VideoFilter: public Filter
{
    DPTR_DECLARE_PRIVATE(Filter)
public:
    VideoFilter();
    virtual ~VideoFilter();

    void installTo(AVPlayer *player);

protected:
    VideoFilter(VideoFilterPrivate *d);
};

}
#endif //PUFF_FILTER_H
