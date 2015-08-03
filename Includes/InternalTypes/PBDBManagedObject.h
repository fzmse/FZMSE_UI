/*
 * PBDBManagedObject.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */
#pragma once

#include <vector>

#include "TinyXML/tinyxml2.h"

#include "ManagedObjectRelativeElement.h"

#include "PBDBManagedObjectParameter.h"
#include "PBDBManagedObjectCompareResult.h"

#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"

#include "Utilities/UtilVector.hpp"

namespace InternalTypes
{
	class PBDBManagedObject : public ManagedObjectRelativeElement
	{
	public:

		const std::string MANAGED_OBJECT_XML_NAME = "managedObject";
		const std::string MANAGED_OBJECT_PARAMETER_XML_NAME = "p";

		PBDBManagedObject(tinyxml2::XMLElement * e);
		~PBDBManagedObject();

		std::vector<PBDBManagedObjectParameter*> getParameters();
		std::vector<PBDBManagedObjectCompareResult> compare(PBDBManagedObject * moc);

		bool isValidMocObject();
	protected:
		PBDBManagedObject();
		std::vector<PBDBManagedObjectParameter*> parameters;

		bool validMocObject;
	};
}
