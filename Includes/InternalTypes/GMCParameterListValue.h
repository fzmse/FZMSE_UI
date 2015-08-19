#pragma once

#include "InternalTypes/GMCParameterDefaultValue.h"

namespace InternalTypes
{
    class GMCParameterListValue : public GMCParameterDefaultValue
    {
    public:
        GMCParameterListValue( tinyxml2::XMLElement * e );
        ~GMCParameterListValue();

        std::vector< std::pair<std::string, std::string> > getSimpleValues();
        std::vector< std::vector< std::pair<std::string, std::string> > > getComplexValues();

    protected:

        void resolveValues();

        std::vector< std::pair<std::string, std::string> > simpleValues;
        std::vector< std::vector< std::pair<std::string, std::string> > > complexValues;

    };
}
