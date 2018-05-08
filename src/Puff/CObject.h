#ifndef PUFF_AVOBJECT_H
#define PUFF_AVOBJECT_H

#include <algorithm>
#include <iostream>
#include <vector>
#include "boost/signals2/signal.hpp"
#include "boost/signals2/signal_type.hpp"

#include "AVGlobal.h"
#include "DPTR.h"

namespace Puff {

using namespace boost;

template <typename S1, class T2, typename S2>
PU_AV_EXPORT void Connect(S1 &s1, T2 *t2, S2 s2)
{
    s1.connect(boost::bind(s2, t2));
};
template <typename S1, class T2, typename S2>
PU_AV_EXPORT void Connect1(S1 &s1, T2 *t2, S2 s2)
{
    s1.connect(boost::bind(s2, t2, _1));
};
template <typename S1, class T2, typename S2>
PU_AV_EXPORT void Connect2(S1 &s1, T2 *t2, S2 s2)
{
    s1.connect(boost::bind(s2, t2, _1, _2));
};
template <typename S1, class T2, typename S2>
PU_AV_EXPORT void Connect3(S1 &s1, T2 *t2, S2 s2)
{
    s1.connect(boost::bind(s2, t2, _1, _2, _3));
};
template <typename S1, typename S2>
PU_AV_EXPORT void Connect(S1 &s1, S2 &s2)
{
    s1.connect(s2);
};

class CObjectPrivate;
class PU_AV_EXPORT CObject
{
    DPTR_DECLARE_PRIVATE(CObject);
public:
    CObject();
    virtual ~CObject();

protected:
    String className() const;
    void setClassName(const String &name);

    void setProperty(const String &key, const String &value);
    String property(const String &key) const;

private:
    DPTR_DECLARE(CObject)
};
}
#endif //PUFF_AVOBJECT_H
