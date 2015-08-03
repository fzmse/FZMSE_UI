/*
 * PBDBManagedObjectParameter.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once

#include <vector>

#include "TinyXML/tinyxml2.h"
#include "ManagedObjectRelativeElement.h"

namespace InternalTypes
{
	class PBDBManagedObjectParameter : public ManagedObjectRelativeElement
	{
	public:
		PBDBManagedObjectParameter( tinyxml2::XMLElement * pElement );
		virtual ~PBDBManagedObjectParameter();

		bool isValidMocParameter();
		std::vector<ManagedObjectRelativeElement *> getProperties();
		std::string getParameterName();

	protected:
		std::vector<ManagedObjectRelativeElement* > properties;
		bool validMocParameter;


	};
}
