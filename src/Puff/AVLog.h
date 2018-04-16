#ifndef PUFF_AVLOG_H
#define PUFF_AVLOG_H

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

class AVLogPrivate;
class PU_AV_EXPORT AVLog
{
    DPTR_DECLARE_PRIVATE(AVLog);
public:
    AVLog *instance();

    void log(LogLevel level, const char *fmt, ...);

    void setLevel(LogLevel level);

    LogLevel level() const;

private:
    explicit AVLog();
    ~AVLog();

private:
    DPTR_DECLARE(AVLog)
};

}
#endif //PUFF_AVLOG_H
