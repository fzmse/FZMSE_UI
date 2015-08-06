/*
 * PDDBManagedObject.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */
#pragma once

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
    class PDDBManagedObject : public ManagedObjectRelativeElement
	{
	public:

		const std::string MANAGED_OBJECT_XML_NAME = "managedObject";
		const std::string MANAGED_OBJECT_PARAMETER_XML_NAME = "p";

        PDDBManagedObject(tinyxml2::XMLElement * e);
        ~PDDBManagedObject();

        std::vector<PDDBManagedObjectParameter*> getParameters();
        std::vector<PDDBManagedObjectCompareResult> compare(PDDBManagedObject * moc);

		bool isValidMocObject();
	protected:
        PDDBManagedObject();
        std::vector<PDDBManagedObjectParameter*> parameters;

		bool validMocObject;
	};
}
