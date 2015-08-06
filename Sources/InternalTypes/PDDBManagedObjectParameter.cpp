/*
 * PDDBManagedObjectParameter.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */
#pragma once

#include "InternalTypes/PDDBManagedObjectParameter.h"

using namespace InternalTypes;
using namespace tinyxml2;
using namespace std;

PDDBManagedObjectParameter::PDDBManagedObjectParameter(XMLElement * e)
	: ManagedObjectRelativeElement::ManagedObjectRelativeElement(e)
{
	this->validMocParameter = false;
	if ( e != NULL )
	{
		this->validMocParameter = true;
		vector<XMLElement * > pElements = XmlReader::getChildren(e, "");
		for ( vector<XMLElement * >::iterator it = pElements.begin(); it != pElements.end(); ++ it )
		{
			this->properties.push_back( new ManagedObjectRelativeElement(*it) );
		}
	}
}

PDDBManagedObjectParameter::~PDDBManagedObjectParameter()
{

}


vector<ManagedObjectRelativeElement* > PDDBManagedObjectParameter::getProperties()
{
	return this->properties;
}

std::string PDDBManagedObjectParameter::getParameterName()
{
	return XmlElementReader::getAttributeByName(this->element, "name");
}
