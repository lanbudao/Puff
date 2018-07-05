#include "SubtitleFilter.h"
#include "Filter_p.h"

namespace Puff {

class SubtitleFilterPrivate: public FilterPrivate
{
public:
    SubtitleFilterPrivate() {

    }
    virtual ~SubtitleFilterPrivate() {

    }

    std::string fileName;
    std::string codec;
};

SubtitleFilter::SubtitleFilter():
    Filter(new SubtitleFilterPrivate)
{

}

SubtitleFilter::~SubtitleFilter()
{

}

void SubtitleFilter::setFile(const string &fileName)
{
    DPTR_D(SubtitleFilter);
    d->fileName = fileName;
}

void SubtitleFilter::setCodec(const string &codec)
{
    DPTR_D(SubtitleFilter);
    d->codec = codec;
}

}
