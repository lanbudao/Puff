#ifndef UTIL_H
#define UTIL_H

#include "AVGlobal.h"
#include <string>

namespace Puff {

class Util
{
public:    
    static std::string guid();

    /*For string option*/
    static std::string sformat(char *fmt, ...);
    static StringList split(const std::string &src, const std::string &delim, bool skipEmpty = false);
    static void push_back(StringList &src, StringList &sub);
};

}
#endif // UTIL_H
