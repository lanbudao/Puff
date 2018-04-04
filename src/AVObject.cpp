#include "AVObject.h"

namespace MSAV {

class AVObjectPrivate: public DptrPrivate<AVObject>
{
public:
    AVObjectPrivate()
    {

    }
    ~AVObjectPrivate()
    {

    }
    String className;
};
AVObject::AVObject()
{
    typedef signal_type<void()> SignalV;
}

AVObject::~AVObject()
{

}

String AVObject::className() const
{
    DPTR_D(const AVObject);
    return d.className;
}

void AVObject::setClassName(const String &name)
{
    DPTR_D(AVObject);
    d.className = name;
}

}
