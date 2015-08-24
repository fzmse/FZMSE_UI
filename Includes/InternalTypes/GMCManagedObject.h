/*
 * GMCManagedObject.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once

#include "InternalTypes/ManagedObjectRelativeElement.h"

#include "InternalTypes/GMCManagedObjectParameter.h"

namespace InternalTypes
{
    class GMCManagedObject : public ManagedObjectRelativeElement
    {
    public:
        const std::string MANAGED_OBJECT_XML_NAME = "managedObject";
        const std::string MANAGED_OBJECT_PARAMETER_XML_NAME = "p";

        GMCManagedObject(tinyxml2::XMLElement * e);
        ~GMCManagedObject();

        void reinitialize();

        std::vector<GMCManagedObjectParameter * > getParameters();
        GMCManagedObjectParameter * getParameterByName(std::string name);

        std::string getClassName();

        bool isValidMocObject();
    protected:
        GMCManagedObject();


        std::vector<GMCManagedObjectParameter * > parameters;

        std::string retrieveClassName(tinyxml2::XMLElement * e);

        std::string className;

        bool validMocObject;

    };
}
