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
    class PDDBManagedObjectParameter : public ManagedObjectRelativeElement
	{
	public:
        PDDBManagedObjectParameter( tinyxml2::XMLElement * pElement );
        virtual ~PDDBManagedObjectParameter();

		bool isValidMocParameter();
		std::vector<ManagedObjectRelativeElement *> getProperties();
		std::string getParameterName();

	protected:
		std::vector<ManagedObjectRelativeElement* > properties;
		bool validMocParameter;


	};
}
