#include "Filter.h"
#include "Filter_p.h"
#include "AVPlayer.h"

namespace Puff {

Filter::Filter(FilterPrivate *d):
    d_ptr(d)
{

}

Filter::~Filter() {

}

bool Filter::enabled() const
{
    DPTR_D(const Filter);
    return d->enabled;
}

void Filter::setEnabled(bool b)
{
    DPTR_D(Filter);
    d->enabled = b;
}

VideoFilter::VideoFilter():
    Filter(new VideoFilterPrivate)
{

}

VideoFilter::VideoFilter(VideoFilterPrivate *d):
    Filter(d)
{

}

VideoFilter::~VideoFilter()
{

}

void VideoFilter::installTo(AVPlayer *player)
{
    player->installFilter(this);
}

AudioFilter::AudioFilter():
    Filter(new AudioFilterPrivate)
{

}

AudioFilter::AudioFilter(AudioFilterPrivate *d):
    Filter(d)
{

}

AudioFilter::~AudioFilter()
{

}

void AudioFilter::installTo(AVPlayer *player)
{
    player->installFilter(this);
}

}
