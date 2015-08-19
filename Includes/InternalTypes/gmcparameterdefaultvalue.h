#pragma once

#include "TinyXML/tinyxml2.h"
#include "Xml/XmlElementReader.h"
#include "Xml/XmlReader.h"
namespace InternalTypes
{
    class GMCParameterDefaultValue
    {
    public:
        GMCParameterDefaultValue(tinyxml2::XMLElement * e);
        virtual ~GMCParameterDefaultValue();
    protected:
        GMCParameterDefaultValue();

        tinyxml2::XMLElement * element;
    };
}
