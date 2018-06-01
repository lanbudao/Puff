#include "AVLog.h"
#include <cstdarg>
#include <direct.h>
#include <vector>
#include "CMutex.h"

namespace Puff {

class AVLogPrivate
{
public:
    AVLogPrivate():
        level(LogAll),
        stream(NULL)
    {
    }
    ~AVLogPrivate()
    {
        if (stream) {
            fclose(stream);
            stream = NULL;
        }
    }

    void writeOutput(const std::string &output)
    {
        if (log_file.empty())
            return;
        WriteLock lock(&mutex);
        if (!stream) {
            stream = fopen(log_file.c_str(), "a+");
            if (!stream)
                return;
        }
        fprintf(stream, "%s", output.c_str());
        fflush(stream);
    }

    CMutex mutex;
    LogLevel level;
    std::string log_file;
    FILE *stream;
};

AVLog::AVLog():
    d_ptr(new AVLogPrivate)
{

}

AVLog::~AVLog() {

}

void AVLog::log(LogLevel level, const char *fmt, ...)
{
    DPTR_D(AVLog);

    if (level > d->level)
        return;
    std::string output;
    va_list marker = NULL;
    va_start(marker, fmt);
    int length = _vscprintf(fmt, marker) + 1;
    std::vector<char> buffer(length, '\0');
    int ret = _vsnprintf(&buffer[0], (size_t)length, fmt, marker);
    if (ret > 0)
        output = &buffer[0];
    va_end(marker);
    printf(output.c_str());
    fflush(stdout);
    d->writeOutput(output);
}

void AVLog::setLevel(LogLevel level)
{
    DPTR_D(AVLog);

    d->level = level;
}

LogLevel AVLog::level() const
{
    DPTR_D(const AVLog);

    return d->level;
}

void AVLog::setLogFile(const std::string &file)
{
    DPTR_D(AVLog);
    if (d->log_file == file) {
        return;
    }
    d->log_file = file;

}

}
