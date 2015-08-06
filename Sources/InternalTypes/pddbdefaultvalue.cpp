#include "InternalTypes/PDDBDefaultValue.h"

using namespace InternalTypes;
using namespace tinyxml2;

PDDBDefaultValue::PDDBDefaultValue(PDDBManagedObjectParameter * p)
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
    this->evaluatedValue = resolveEvaluatedValue(p);

}

std::string PDDBDefaultValue::resolveTypeName(PDDBManagedObjectParameter * p)
{
    if ( p == NULL )
        return NULL;

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttribute(p->getElement(),
                                                       "simpleType");
    if ( elems.size() == 0 )
        return NULL;

    XMLElement * e = elems[0];

    if ( e == NULL )
        return NULL;

    std::string attribValue = XmlElementReader::getAttributeByName(e, "base");

    return attribValue;
}

std::vector<std::pair<std::string, std::string> > PDDBDefaultValue::resolveEnums(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return results;

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttribute(p->getElement(),
                                                       "simpleType");

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

std::string PDDBDefaultValue::resolveEvaluation(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return "";

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttribute(p->getElement(),
                                                       "simpleType");

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

std::string PDDBDefaultValue::resolveValue(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return "";

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttribute(p->getElement(),
                                                       "simpleType");

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



std::string PDDBDefaultValue::resolveEvaluatedValue(PDDBManagedObjectParameter * p)
{
    // check evaluation, enums and things like that
    Parser parser;

    if ( this->enums.size() != 0 )
    {
        // Look for enums

    }

    if ( this->evaluation != "" && this->value != "")
    {
        // Calculate expression
        std::string equatedValue = extractAnswerFromExpression(this->getEvaluation(), this->getValue());
        if ( equatedValue != "" )
            return equatedValue;
    }

    return "";
}

std::vector<std::pair<std::string, std::string> > PDDBDefaultValue::resolveRange(PDDBManagedObjectParameter * p)
{
    std::vector<std::pair<std::string, std::string> > results;

    if ( p == NULL )
        return results;

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttribute(p->getElement(),
                                                       "simpleType");

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


std::string PDDBDefaultValue::getTypeName()
{
    return this->typeName;
}

std::vector<std::pair<std::string, std::string> > PDDBDefaultValue::getEnums()
{
    return this->enums;
}

std::string PDDBDefaultValue::getEvaluation()
{
    return this->evaluation;
}

std::string PDDBDefaultValue::getValue()
{
    return this->value;
}

std::string PDDBDefaultValue::getEvaluatedValue()
{
    return this->evaluatedValue;
}


bool PDDBDefaultValue::isValueAvailable()
{
    return this->valueAvailable;
}

std::vector<std::pair<std::string, std::string> > PDDBDefaultValue::getRange()
{
    return this->range;
}
