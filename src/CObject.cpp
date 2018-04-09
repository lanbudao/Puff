#include "CObject.h"

#include "boost/thread.hpp"
#include "boost/bind.hpp"

namespace MSAV {

class CObjectPrivate: public DptrPrivate<CObject>
{
public:
    CObjectPrivate()
    {

    }
    ~CObjectPrivate()
    {

    }
    String className;
};
CObject::CObject()
{

}

CObject::~CObject()
{

}

String CObject::className() const
{
    DPTR_D(const CObject);
    return d.className;
}

void CObject::setClassName(const String &name)
{
    DPTR_D(CObject);
    d.className = name;
}

}
