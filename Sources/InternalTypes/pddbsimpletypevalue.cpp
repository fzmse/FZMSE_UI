#include "Includes/InternalTypes/pddbsimpletypevalue.h"

using namespace InternalTypes;
using namespace tinyxml2;

PDDBSimpleTypeValue::PDDBSimpleTypeValue(PDDBManagedObjectParameter * p)
{
    this->valueAvailable = true;

    // Retrieve Type Name
    std::string retrievedTypeName = resolveTypeName(p);
    if ( retrievedTypeName == "")
        this->valueAvailable &= false;
    else
        this->typeName = retrievedTypeName;

    // Retrieve enums
    this->enums = resolveEnums(p);

    // Retrieve range
    this->range = resolveRange(p);

    // Retrieve Evaluation
    this->evaluation = resolveEvaluation(p);

    // Retrieve value
    this->value = resolveValue(p);

    // Evaluate value
    this->evaluatedValue = resolveEvaluatedValue();

}

bool PDDBSimpleTypeValue::isComplexType()
{
    return false;
}

std::string PDDBSimpleTypeValue::resolveTypeName(PDDBManagedObjectParameter * p)
{
    if ( p == NULL )
        return "";

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(p->getElement(), "simpleType");
    if ( elems.size() == 0 )
        return "";

    XMLElement * e = elems[0];

    if ( e == NULL )
        return "";

    std::string attribValue = XmlElementReader::getAttributeByName(e, "base");

    return attribValue;
}

std::vector<std::pair<std::string, std::string> > PDDBSimpleTypeValue::resolveEnums(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return results;

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(p->getElement(), "simpleType");

    if ( elems.size() == 0 )
        return results;

    XMLElement * e = elems[0];

    if ( e == NULL )
        return results;

    std::vector<XMLElement *> enumsElems = XmlReader::getElementsWithSpecificNameAndAttribute(e,
                                                       "enumeration");
    for ( std::vector<XMLElement *>::iterator it = enumsElems.begin(); it != enumsElems.end(); ++ it)
    {
        std::pair<std::string, std::string> entry;
        entry.first = XmlElementReader::getAttributeByName(*it, "value");
        entry.second = XmlElementReader::getAttributeByName(*it, "text");

        results.push_back(entry);
    }
}

std::string PDDBSimpleTypeValue::resolveEvaluation(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return "";

    std::vector<XMLElement *> elems =  XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(p->getElement(), "simpleType");

    if ( elems.size() == 0 )
        return "";

    XMLElement * e = elems[0];

    if ( e == NULL )
        return "";

    std::vector<XMLElement *> editingElems = XmlReader::getElementsWithSpecificNameAndAttribute(e,
                                                       "editing");
    if ( editingElems.size() == 0 )
        return "";

    return XmlElementReader::getAttributeByName(editingElems[0], "internalValue" );
}

std::string PDDBSimpleTypeValue::resolveValue(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return "";

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(p->getElement(), "simpleType");

    if ( elems.size() == 0 )
        return "";

    XMLElement * e = elems[0];

    if ( e == NULL )
        return "";

    std::vector<XMLElement *> defValElems = XmlReader::getElementsWithSpecificNameAndAttribute(e,
                                                       "default");
    if ( defValElems.size() == 0 )
        return "";

    return XmlElementReader::getAttributeByName(defValElems[0], "value" );
}



std::string PDDBSimpleTypeValue::resolveEvaluatedValue()
{
    // check evaluation, enums and things like that
    Parser parser;

    if ( this->enums.size() != 0 )
    {
        // Look for enums
        for ( std::vector<std::pair<std::string, std::string> >::iterator it = this->enums.begin();
              it != this->enums.end(); ++ it)
        {
            if ( (*it).first == this->value )
                return (*it).second;
        }
    }

    if ( this->evaluation != "" && this->value != "")
    {
        // Calculate expression
        std::string equatedValue = extractAnswerFromExpression(this->getEvaluation(), this->getValue());
        if ( equatedValue != "" )
            return equatedValue;
    }

    // Return raw value if no modifiers are present
    return this->value;
}

std::vector<std::pair<std::string, std::string> > PDDBSimpleTypeValue::resolveRange(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return results;

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(p->getElement(), "simpleType");

    if ( elems.size() == 0 )
        return results;

    XMLElement * e = elems[0];

    if ( e == NULL )
        return results;

    std::vector<XMLElement *> rangeElems = XmlReader::getElementsWithSpecificNameAndAttribute(e,
                                                       "range");
    for ( std::vector<XMLElement *>::iterator it = rangeElems.begin(); it != rangeElems.end(); ++ it)
    {
        std::pair<std::string, std::string> entry;
        entry.first = XmlElementReader::getAttributeByName(*it, "minIncl");
        entry.second = XmlElementReader::getAttributeByName(*it, "maxIncl");

        results.push_back(entry);
    }
}


std::string PDDBSimpleTypeValue::getTypeName()
{
    return this->typeName;
}

std::vector<std::pair<std::string, std::string> > PDDBSimpleTypeValue::getEnums()
{
    return this->enums;
}

std::string PDDBSimpleTypeValue::getEvaluation()
{
    return this->evaluation;
}

std::string PDDBSimpleTypeValue::getValue()
{
    return this->value;
}

std::string PDDBSimpleTypeValue::getEvaluatedValue()
{
    return this->evaluatedValue;
}


bool PDDBSimpleTypeValue::isValueAvailable()
{
    return this->valueAvailable;
}

std::vector<std::pair<std::string, std::string> > PDDBSimpleTypeValue::getRange()
{
    return this->range;
}
