#include "AVObject.h"

namespace MSAV {

class AVObject::Private
{
public:
    Private()
    {

    }
    ~Private()
    {

    }
    String className;
};
AVObject::AVObject():
    d(new Private())
{
    typedef signal_type<void()> SignalV;
}

AVObject::~AVObject()
{
    delete d;
}

String AVObject::className() const
{
    return d->className;
}

void AVObject::setClassName(const String &name)
{
    d->className = name;
}

}
