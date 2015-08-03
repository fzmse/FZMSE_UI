/*
 * ManagedObjectRelativeElement.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#include "InternalTypes/ManagedObjectRelativeElement.h"

using namespace InternalTypes;
using namespace tinyxml2;
using namespace std;

ManagedObjectRelativeElement::ManagedObjectRelativeElement(XMLElement * e)
{
	this->element = e;

	vector<pair<string, string> > a = XmlElementReader::getAttributes(e);
	for ( vector<pair<string, string> >::iterator it = a.begin(); it != a.end(); ++ it )
		this->attributes.push_back(*it);
}

ManagedObjectRelativeElement::~ManagedObjectRelativeElement()
{

}


void ManagedObjectRelativeElement::setAttributes( std::vector<Attribute> a)
{
	this->attributes = a;
}

std::vector<Attribute> ManagedObjectRelativeElement::getAttributes()
{
	return this->attributes;
}

XMLElement * ManagedObjectRelativeElement::getElement()
{
	return this->element;
}
