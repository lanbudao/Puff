#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace Puff {

class Util
{
public:
    static std::string sformat(char *fmt, ...);

    static std::string guid();

};

}
#endif // UTIL_H
