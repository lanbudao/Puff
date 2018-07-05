#ifndef PUFF_SUBTITLE_FILTER_H
#define PUFF_SUBTITLE_FILTER_H

#include "Filter.h"

namespace Puff {

class SubtitleFilterPrivate;
class PU_AV_EXPORT SubtitleFilter: public Filter
{
    DPTR_DECLARE_PRIVATE(SubtitleFilter)
public:
    SubtitleFilter();
    ~SubtitleFilter();

    void setFile(const std::string &fileName);
    void setCodec(const std::string &codec);

};

}
#endif //PUFF_SUBTITLE_FILTER_H
