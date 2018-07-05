#ifndef PUFF_FILTER_P_H
#define PUFF_FILTER_P_H

namespace Puff {

class FilterPrivate
{
public:
    FilterPrivate():
        enabled(true)
    {

    }
    virtual ~FilterPrivate()
    {

    }

    bool enabled;
};

class AudioFilterPrivate: public FilterPrivate
{
public:
    AudioFilterPrivate()
    {

    }
    virtual ~AudioFilterPrivate()
    {

    }
};

class VideoFilterPrivate: public FilterPrivate
{
public:
    VideoFilterPrivate()
    {

    }
    virtual ~VideoFilterPrivate()
    {

    }
};

}
#endif //PUFF_FILTER_P_H
