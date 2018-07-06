#ifndef PUFF_SUBTITLE_H
#define PUFF_SUBTITLE_H

#include "AVGlobal.h"
#include "DPTR.h"
#include "ByteArray.h"

namespace Puff {

enum SubtitleType {
    SubtitleText,
    SubtitleAss,
    SubtitlePixmap
};

struct SubtitleFrame {
    SubtitleFrame():
        begin(0.0),
        end(0.0),
        type(SubtitleText)
    {

    }

    bool isValid() const {return begin < end;}
    operator bool() const {return isValid();}
    inline bool operator < (const SubtitleFrame &f) const {return end < f.end;}
    inline bool operator < (double t) const {return end < t;}

    SubtitleType type;
    double begin, end;
    std::string text;
};

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
    bool processLine(const ByteArray &data, double pts, double duration);

protected:

private:
    DPTR_DECLARE(Subtitle)
};

}
#endif //PUFF_SUBTITLE_H
