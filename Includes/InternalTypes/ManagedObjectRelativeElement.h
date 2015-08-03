/*
 * ManagedObjectRelativeElement.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once

#include <vector>

#include "TinyXML/tinyxml2.h"

#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"

#include "Attribute.h"

namespace InternalTypes
{
	class ManagedObjectRelativeElement
	{
	public:
		ManagedObjectRelativeElement(tinyxml2::XMLElement * e);
		virtual ~ManagedObjectRelativeElement();

		std::vector<Attribute> getAttributes();
		std::vector<tinyxml2::XMLElement *> getChildXmlElements();
		tinyxml2::XMLElement * getElement();

		void setAttributes( std::vector<Attribute> a);

	protected:
		tinyxml2::XMLElement * element;
		std::vector<Attribute> attributes;
	};
}
