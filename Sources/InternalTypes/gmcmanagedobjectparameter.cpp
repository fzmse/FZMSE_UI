
#include "InternalTypes/GMCManagedObjectParameter.h"

using namespace std;
using namespace tinyxml2;
using namespace InternalTypes;


GMCManagedObjectParameter::GMCManagedObjectParameter( XMLElement * e, ManagedObjectRelativeElement * parentMoc ) :
    ManagedObjectRelativeElement(e)
{
    this->parentMoc = parentMoc;

    this->name = retrieveName();

    this->defaultValue = retrieveValue();
}

GMCManagedObjectParameter::~GMCManagedObjectParameter()
{
    if ( this->defaultValue != NULL )
        delete this->defaultValue;
}

std::string GMCManagedObjectParameter::getName()
{
    return this->name;
}

std::string GMCManagedObjectParameter::retrieveName()
{
    return XmlElementReader::getAttributeByName( this->element, "name");
}

GMCManagedObjectParameter::ChildValueType GMCManagedObjectParameter::getType()
{
    return this->valueType;
}

GMCParameterDefaultValue * GMCManagedObjectParameter::getValue()
{
    return this->defaultValue;
}

GMCParameterDefaultValue * GMCManagedObjectParameter::retrieveValue()
{
    std::string parTypeName = XmlElementReader::getName(this->element);

    GMCParameterDefaultValue * resultValue = NULL;


    if ( parTypeName == "list" )
    {
        this->valueType = List;
        resultValue = new GMCParameterListValue(this->element);
    }

    if ( parTypeName == "p" )
    {
        this->valueType = SingleParameter;
        resultValue = new GMCParameterSingleValue(this->element);
    }

    return resultValue;
}

ManagedObjectRelativeElement * GMCManagedObjectParameter::getParentMoc()
{
    return this->parentMoc;
}
