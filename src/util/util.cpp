#include "util.h"

#include <cstdarg>
#include <vector>
#include <time.h>

namespace Puff {

std::string sformat(char *fmt, ...)
{
    std::string output;
    va_list marker = NULL;
    va_start(marker, fmt);
    int length = _vscprintf(fmt, marker) + 1;
    std::vector<char> buffer(length, '\0');
    int ret = _vsnprintf(&buffer[0], (size_t)length, fmt, marker);
    if (ret > 0)
        output = &buffer[0];
    va_end(marker);
    return output;
}

std::string guid()
{
    char range[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int length = strlen(range);
    static bool s = false;
    if (!s) {
        unsigned int seed = (unsigned)time(NULL);
        srand(seed);
        s = true;
    }
    char uid[17];
    memset(uid, 0, 17);
    for (int i=0; i < 16; i++) {
        int t = rand() % length;
        uid[i] = range[t];
    }
    return std::string(uid);
}

}
