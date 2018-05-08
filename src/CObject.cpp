#include "CObject.h"

#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include <map>

namespace Puff {

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
    std::map<String, String> properties;
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

void CObject::setProperty(const String &key, const String &value) {
    DPTR_D(CObject);
    d.properties.insert(std::map<String, String>::value_type(key, value));
}

String CObject::property(const String &key) const {
    DPTR_D(const CObject);
    std::map<String, String>::const_iterator it = d.properties.find(key);
    if (it != d.properties.end()) {
        return it->second;
    }
    return String("");
}

}
