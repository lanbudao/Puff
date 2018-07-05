#include "util.h"

#include <cstdarg>
#include <vector>
#include <time.h>

namespace Puff {

std::string Util::guid()
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

std::string Util::sformat(char *fmt, ...)
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

StringList Util::split(const std::string &src, const std::string &delim, bool skipEmpty)
{
    StringList result;
    size_t prior_pos = 0;
    size_t pos = src.find(delim);
    std::string sub;

    if (pos == std::string::npos)
        return result;
    while (pos != std::string::npos) {
        sub = src.substr(prior_pos, pos);
        if (!(sub.empty() && skipEmpty)) {
            result.push_back(sub);
        }
        pos = src.find(delim);
        prior_pos = pos;
    }
    sub = src.substr(prior_pos, pos);
    if (!(sub.empty() && skipEmpty)) {
        result.push_back(sub);
    }
    return result;
}

void Util::push_back(StringList &src, StringList &sub)
{
    StringList::iterator itor = sub.begin();
    for (; itor != sub.end(); itor++) {
        src.push_back(*itor);
    }
}

}
