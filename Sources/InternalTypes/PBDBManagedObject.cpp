/*
 * PBDBManagedObject.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#include "InternalTypes/PBDBManagedObject.h"

using namespace std;

using namespace InternalTypes;
using namespace tinyxml2;

PBDBManagedObject::PBDBManagedObject(XMLElement * e)
	: ManagedObjectRelativeElement::ManagedObjectRelativeElement(e)
{
	this->validMocObject = false;
	if ( e != NULL )
	{
		if ( XmlElementReader::getName(e) == MANAGED_OBJECT_XML_NAME )
		{
			this->validMocObject = true;

			vector<XMLElement * > pElements = XmlReader::getElementsWithSpecificNameAndAttribute(e, "");
			for ( vector<XMLElement * >::iterator it = pElements.begin(); it != pElements.end(); ++ it )
			{
				if ( XmlElementReader::getName(*it) == MANAGED_OBJECT_PARAMETER_XML_NAME )
					this->parameters.push_back( new PBDBManagedObjectParameter(*it) );
			}

		}

	}
}

PBDBManagedObject::~PBDBManagedObject()
{
	for ( vector<PBDBManagedObjectParameter *>::iterator it = this->parameters.begin();
			it != this->parameters.end(); ++ it )
	{
		delete (*it);
	}
}

bool PBDBManagedObject::isValidMocObject()
{
	return this->validMocObject;
}

vector<PBDBManagedObjectParameter *> PBDBManagedObject::getParameters()
{
	return this->parameters;
}

inline PBDBManagedObjectParameter* getParameterByNameFrom(std::string name, vector<PBDBManagedObjectParameter*> list)
{
	for ( vector<PBDBManagedObjectParameter*>::iterator it = list.begin();
		it != list.end(); ++ it )
	{
		if ( (*it)->getParameterName() == name )
			return (*it);
	}
	return NULL;
}

vector<PBDBManagedObjectCompareResult> PBDBManagedObject::compare(PBDBManagedObject * moc)
{
	vector<PBDBManagedObjectCompareResult> differences;

	// checkMoc
	vector<Attribute> firstMocAttributes = this->getAttributes();
	vector<Attribute> secondMocAttributes = moc->getAttributes();

	// Get moc attribute difference list
	bool mocAttributesIdentical = isVectorsIdentical(firstMocAttributes, secondMocAttributes);
	if ( mocAttributesIdentical == false )
	{
		vector<AttributeDifference> diffs = AttributeDifference::getDifferentVectorsOfAttributes(firstMocAttributes,
						secondMocAttributes);
		differences.push_back(PBDBManagedObjectCompareResult(PBDBManagedObjectCompareResult::AttributeDifference,
				PBDBManagedObjectCompareResult::ManagedObject,
				PBDBManagedObjectCompareResult::Modified,
				this, moc,
				diffs
		));
	}

	// get parameters difference list
	vector<PBDBManagedObjectParameter*> firstParameters = this->getParameters();
	vector<PBDBManagedObjectParameter*> secondParameters = moc->getParameters();

	for ( vector<PBDBManagedObjectParameter*>::iterator it = firstParameters.begin();
			it != firstParameters.end(); ++ it )
	{
		// find parameter by class
		PBDBManagedObjectParameter* match = getParameterByNameFrom((*it)->getParameterName(), secondParameters);
		if ( match == NULL )
		{
			// removed
			differences.push_back(PBDBManagedObjectCompareResult(PBDBManagedObjectCompareResult::AttributeDifference,
							PBDBManagedObjectCompareResult::ManagedObjectParameter,
							PBDBManagedObjectCompareResult::Removed,
							(*it), match
				));
		}
		else
		{
			// -------------------->             check for modified
			PBDBManagedObjectParameter* firstParameter = (*it);
			PBDBManagedObjectParameter* secondParameter = match;

			vector<Attribute> firstMocParameterAttributes = firstParameter->getAttributes();
			vector<Attribute> secondMocParameterAttributes = secondParameter->getAttributes();

			bool mocAttributesIdentical = isVectorsIdentical(firstMocParameterAttributes, secondMocParameterAttributes);
			if ( mocAttributesIdentical == false )
			{
				vector<AttributeDifference> diffs = AttributeDifference::getDifferentVectorsOfAttributes(firstMocParameterAttributes,
						secondMocParameterAttributes);
				differences.push_back(PBDBManagedObjectCompareResult(PBDBManagedObjectCompareResult::AttributeDifference,
						PBDBManagedObjectCompareResult::ManagedObjectParameter,
						PBDBManagedObjectCompareResult::Modified,
						this, moc,
						diffs
				));
			}


			// ------------------>             check for modified properties

			//   .................................................. TODO

		}
	}

	for ( vector<PBDBManagedObjectParameter*>::iterator it = secondParameters.begin();
					it != secondParameters.end(); ++ it )
	{
		PBDBManagedObjectParameter* match = getParameterByNameFrom((*it)->getParameterName(), firstParameters);
		if ( match == NULL )
		{
			// added
			differences.push_back(PBDBManagedObjectCompareResult(PBDBManagedObjectCompareResult::AttributeDifference,
						PBDBManagedObjectCompareResult::ManagedObjectParameter,
						PBDBManagedObjectCompareResult::Added,
						match, (*it)
			));
		}
	}

	return differences;
}
