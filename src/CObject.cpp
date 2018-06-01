#include "CObject.h"

#include <map>

namespace Puff {

class CObjectPrivate
{
public:
    CObjectPrivate()
    {

    }
    ~CObjectPrivate()
    {

    }
    std::string className;
    std::map<std::string, std::string> properties;
};

CObject::CObject():
    d_ptr(new CObjectPrivate)
{

}

CObject::~CObject()
{

}

std::string CObject::className() const
{
    DPTR_D(const CObject);
    return d->className;
}

void CObject::setClassName(const std::string &name)
{
    DPTR_D(CObject);
    d->className = name;
}

void CObject::setProperty(const std::string &key, const std::string &value) {
    DPTR_D(CObject);
    d->properties.insert(std::map<std::string, std::string>::value_type(key, value));
}

std::string CObject::property(const std::string &key) const {
    DPTR_D(const CObject);
    std::map<std::string, std::string>::const_iterator it = d->properties.find(key);
    if (it != d->properties.end()) {
        return it->second;
    }
    return std::string("");
}

}
