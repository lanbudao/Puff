#ifndef PUFF_AVLOG_H
#define PUFF_AVLOG_H

#include "AVGlobal.h"
#include "DPTR.h"
#include "Singleton.h"

namespace Puff {

#define debug(fmt, ...) AVLog::instance().log(LogDebug, fmt, __VA_ARGS__)
#define warnning(fmt, ...) AVLog::instance().log(LogWarning, fmt, __VA_ARGS__)
#define error(fmt, ...) AVLog::instance().log(LogError, fmt, __VA_ARGS__)

class AVLogPrivate;
class PU_AV_EXPORT AVLog: public Singleton<AVLog>
{
    DPTR_DECLARE_PRIVATE(AVLog)
    DISABLE_COPY(AVLog)
public:
    void log(LogLevel level, const char *fmt, ...);

    void setLevel(LogLevel level);

    LogLevel level() const;

public:
    explicit AVLog();
    ~AVLog();

private:
    DPTR_DECLARE(AVLog)
};

}
#endif //PUFF_AVLOG_H
