#ifndef MSAV_AVOBJECT_H
#define MSAV_AVOBJECT_H

#include <algorithm>
#include <iostream>
#include <vector>
#include "signals2/signal.hpp"
#include "signals2/signal_type.hpp"

#include "MSAVGlobal.h"
#include "DPTR.h"

namespace MSAV {

using namespace boost;

template <typename S1, class T2, typename S2>
static inline void Connect(S1 &s1, T2 *t2, S2 s2)
{
    s1.connect(boost::bind(s2, t2));
};
template <typename S1, typename S2>
static inline void Connect(S1 &s1, S2 &s2)
{
    s1.connect(s2);
};

class CObjectPrivate;
class CObject
{
    DPTR_DECLARE_PRIVATE(CObject);
public:
    CObject();
    virtual ~CObject();

protected:
    String className() const;
    void setClassName(const String &name);

private:
};
}
#endif //MSAV_AVOBJECT_H
