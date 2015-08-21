#pragma once

#include "InternalTypes/GMCParameterDefaultValue.h"

namespace InternalTypes
{
    class GMCParameterSingleValue : public GMCParameterDefaultValue
    {
    public :
        GMCParameterSingleValue(tinyxml2::XMLElement * e);
        ~GMCParameterSingleValue();

        std::string getValue();
    protected:

        std::string retrieveValue();

        std::string value;
    };
}
