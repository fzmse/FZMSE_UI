/*
 * PDDBManagedObject.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */
#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "TinyXML/tinyxml2.h"

#include "ManagedObjectRelativeElement.h"

#include "PDDBManagedObjectParameter.h"
#include "PDDBManagedObjectCompareResult.h"

#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"

#include "Utilities/UtilVector.hpp"

namespace InternalTypes
{
    class PDDBManagedObjectParameter;

    class PDDBManagedObject : public ManagedObjectRelativeElement
	{
	public:

        const std::string MANAGED_OBJECT_XML_NAME = "managedObject";
        const std::string MANAGED_OBJECT_PARAMETER_XML_NAME = "p";

        PDDBManagedObject(tinyxml2::XMLElement * e, std::string version);
        ~PDDBManagedObject();

        std::vector<PDDBManagedObjectCompareResult> compare(PDDBManagedObject * moc);

        std::vector<PDDBManagedObjectParameter*> getParameters();
        std::string getClassName();



        PDDBManagedObjectParameter * getMocIdParameter();

        std::string getVersion();

		bool isValidMocObject();
	protected:
        PDDBManagedObject();

        std::string retrieveClassName(tinyxml2::XMLElement * e);
        PDDBManagedObjectParameter * retrieveMocIdParameter();



        std::vector<PDDBManagedObjectParameter*> parameters; // TODO

        PDDBManagedObjectParameter * mocIdParameter;

        std::string className;
        std::string version;

		bool validMocObject;


	};
}
