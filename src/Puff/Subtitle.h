#ifndef PUFF_SUBTITLE_H
#define PUFF_SUBTITLE_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class SubtitlePrivate;
class PU_AV_EXPORT Subtitle
{
    DPTR_DECLARE_PRIVATE(Subtitle)
public:
    Subtitle();
    ~Subtitle();

private:
    DPTR_DECLARE(Subtitle)
};

}
#endif //PUFF_SUBTITLE_H
