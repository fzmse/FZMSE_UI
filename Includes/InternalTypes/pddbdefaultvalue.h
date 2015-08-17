#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "TinyXML/tinyxml2.h"

#include "ExpressionParser/parser.h"

#include "Utilities/UtilExpressionParser.hpp"

#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"

namespace InternalTypes
{
    class PDDBDefaultValue
    {
    public:
        virtual ~PDDBDefaultValue() { }
        virtual bool isComplexType() = 0;
    };
}
