#ifndef PUFF_SUBTITLE_FILTER_H
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

    bool enabled() const;
    void setEnabled(bool b);
    void setFile(const std::string &fileName);
    void setCodec(const std::string &codec);
    void load();

protected:

private:
    DPTR_DECLARE(Subtitle)
};

}
#endif //PUFF_SUBTITLE_FILTER_H
