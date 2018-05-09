#ifndef PUFF_AVLOG_H
#define PUFF_AVLOG_H

#include "DPTR.h"
#include "Singleton.h"

namespace Puff {

#define logger AVLog::instance()
#define avdebug(fmt, ...) logger.log(LogDebug, fmt, ##__VA_ARGS__)
#define avwarnning(fmt, ...) logger.log(LogWarning, fmt, ##__VA_ARGS__)
#define averror(fmt, ...) logger.log(LogError, fmt, ##__VA_ARGS__)

/**
 * @brief If you need to print the output to the local file, you need to
 * call 'setLogFile' to specify the file to print.
 */
class AVLogPrivate;
class PU_AV_EXPORT AVLog: public Singleton<AVLog>
{
    DPTR_DECLARE_PRIVATE(AVLog)
    DISABLE_COPY(AVLog)
public:
    void setLogFile(const String &file);

    void log(LogLevel level, const char *fmt, ...);

    void setLevel(LogLevel level);

    LogLevel level() const;

public:
    AVLog();
    ~AVLog();

private:
    DPTR_DECLARE(AVLog)
};

}
#endif //PUFF_AVLOG_H
