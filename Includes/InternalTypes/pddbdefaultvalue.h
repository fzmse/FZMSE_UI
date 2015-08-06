#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "TinyXML/tinyxml2.h"

#include "ExpressionParser/parser.h"

#include "Utilities/UtilExpressionParser.hpp"

#include "InternalTypes/PDDBManagedObjectParameter.h"

#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"

namespace InternalTypes
{
    class PDDBDefaultValue
    {
    public:
        PDDBDefaultValue(PDDBManagedObjectParameter * p);

        std::string getTypeName();
        std::vector<std::pair<std::string, std::string> > getEnums();
        std::string getEvaluation();
        std::string getValue();
        std::string getEvaluatedValue();
        std::vector<std::pair<std::string, std::string> > getRange();

        bool isValueAvailable();
    protected:

        std::string resolveTypeName(PDDBManagedObjectParameter * p);
        std::vector<std::pair<std::string, std::string> > resolveEnums(PDDBManagedObjectParameter * p);
        std::string resolveEvaluation(PDDBManagedObjectParameter * p);
        std::string resolveValue(PDDBManagedObjectParameter * p);
        std::string resolveEvaluatedValue(PDDBManagedObjectParameter * p);
        std::vector<std::pair<std::string, std::string> > resolveRange(PDDBManagedObjectParameter * p);

        std::string typeName;
        std::vector<std::pair<std::string, std::string> > enums;
        std::string value;
        std::string evaluatedValue;
        std::string evaluation;
        std::vector< std::pair<std::string, std::string> > range;

        // if the value can be determined
        bool valueAvailable;

    };
}
