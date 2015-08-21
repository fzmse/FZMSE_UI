
#include "InternalTypes/GMCParameterSingleValue.h"

using namespace InternalTypes;
using namespace std;
using namespace tinyxml2;

GMCParameterSingleValue::GMCParameterSingleValue(XMLElement *e) :
    GMCParameterDefaultValue(e)
{
    this->value = retrieveValue();
}

GMCParameterSingleValue::~GMCParameterSingleValue()
{

}

std::string GMCParameterSingleValue::getValue()
{
    return this->value;
}

std::string GMCParameterSingleValue::retrieveValue()
{
    const char * res = this->element->GetText();
    return res == NULL ? "" : res ;
}
