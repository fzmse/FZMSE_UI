#pragma once

#include "Xml/XmlElementReader.h"

#include "InternalTypes/ManagedObjectRelativeElement.h"

#include "InternalTypes/GMCParameterDefaultValue.h"
#include "InternalTypes/GMCParameterSingleValue.h"
#include "InternalTypes/GMCParameterListValue.h"

namespace InternalTypes
{
    class GMCManagedObjectParameter : public ManagedObjectRelativeElement
    {
    public:
        enum ChildValueType
        {
            SingleParameter,
            List
        };


        GMCManagedObjectParameter(tinyxml2::XMLElement * e, ManagedObjectRelativeElement * parentMoc);
        ~GMCManagedObjectParameter();

        ChildValueType getType();

        ManagedObjectRelativeElement * getParentMoc();

        std::string getName();
        GMCParameterDefaultValue * getValue();

    protected:

        ManagedObjectRelativeElement * parentMoc;

        std::string retrieveName();
        GMCParameterDefaultValue * retrieveValue();

        ChildValueType valueType;

        GMCParameterDefaultValue * defaultValue;

        std::string name;



    };
}
