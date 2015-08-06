/*
 * PDDBManagedObject.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#include "InternalTypes/PDDBManagedObject.h"

using namespace std;

using namespace InternalTypes;
using namespace tinyxml2;

PDDBManagedObject::PDDBManagedObject(XMLElement * e)
	: ManagedObjectRelativeElement::ManagedObjectRelativeElement(e)
{
	this->validMocObject = false;
	if ( e != NULL )
	{
		if ( XmlElementReader::getName(e) == MANAGED_OBJECT_XML_NAME )
		{
			this->validMocObject = true;

            vector<XMLElement * > pElements = XmlReader::getChildren(e, MANAGED_OBJECT_PARAMETER_XML_NAME);
			for ( vector<XMLElement * >::iterator it = pElements.begin(); it != pElements.end(); ++ it )
			{
				if ( XmlElementReader::getName(*it) == MANAGED_OBJECT_PARAMETER_XML_NAME )
                    this->parameters.push_back( new PDDBManagedObjectParameter(*it) );
			}

		}

	}
}

PDDBManagedObject::~PDDBManagedObject()
{
    for ( vector<PDDBManagedObjectParameter *>::iterator it = this->parameters.begin();
			it != this->parameters.end(); ++ it )
	{
		delete (*it);
	}
}

bool PDDBManagedObject::isValidMocObject()
{
	return this->validMocObject;
}

vector<PDDBManagedObjectParameter *> PDDBManagedObject::getParameters()
{
	return this->parameters;
}

inline PDDBManagedObjectParameter* getParameterByNameFrom(std::string name, vector<PDDBManagedObjectParameter*> list)
{
    for ( vector<PDDBManagedObjectParameter*>::iterator it = list.begin();
		it != list.end(); ++ it )
	{
		if ( (*it)->getParameterName() == name )
			return (*it);
	}
	return NULL;
}

vector<PDDBManagedObjectCompareResult> PDDBManagedObject::compare(PDDBManagedObject * moc)
{
    vector<PDDBManagedObjectCompareResult> differences;

	// checkMoc
	vector<Attribute> firstMocAttributes = this->getAttributes();
	vector<Attribute> secondMocAttributes = moc->getAttributes();

	// Get moc attribute difference list
	bool mocAttributesIdentical = isVectorsIdentical(firstMocAttributes, secondMocAttributes);
	if ( mocAttributesIdentical == false )
	{
		vector<AttributeDifference> diffs = AttributeDifference::getDifferentVectorsOfAttributes(firstMocAttributes,
						secondMocAttributes);
        differences.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::AttributeDifference,
                PDDBManagedObjectCompareResult::ManagedObject,
                PDDBManagedObjectCompareResult::Modified,
				this, moc,
				diffs
		));
	}

	// get parameters difference list
    vector<PDDBManagedObjectParameter*> firstParameters = this->getParameters();
    vector<PDDBManagedObjectParameter*> secondParameters = moc->getParameters();

    for ( vector<PDDBManagedObjectParameter*>::iterator it = firstParameters.begin();
			it != firstParameters.end(); ++ it )
	{
		// find parameter by class
        PDDBManagedObjectParameter* match = getParameterByNameFrom((*it)->getParameterName(), secondParameters);
		if ( match == NULL )
		{
			// removed
            differences.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::AttributeDifference,
                            PDDBManagedObjectCompareResult::ManagedObjectParameter,
                            PDDBManagedObjectCompareResult::Removed,
							(*it), match
				));
		}
		else
		{
			// -------------------->             check for modified
            PDDBManagedObjectParameter* firstParameter = (*it);
            PDDBManagedObjectParameter* secondParameter = match;

			vector<Attribute> firstMocParameterAttributes = firstParameter->getAttributes();
			vector<Attribute> secondMocParameterAttributes = secondParameter->getAttributes();

			bool mocAttributesIdentical = isVectorsIdentical(firstMocParameterAttributes, secondMocParameterAttributes);
			if ( mocAttributesIdentical == false )
			{
				vector<AttributeDifference> diffs = AttributeDifference::getDifferentVectorsOfAttributes(firstMocParameterAttributes,
						secondMocParameterAttributes);
                differences.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::AttributeDifference,
                        PDDBManagedObjectCompareResult::ManagedObjectParameter,
                        PDDBManagedObjectCompareResult::Modified,
                        firstParameter, secondParameter,
						diffs
				));
			}


			// ------------------>             check for modified properties

			//   .................................................. TODO

		}
	}

    for ( vector<PDDBManagedObjectParameter*>::iterator it = secondParameters.begin();
					it != secondParameters.end(); ++ it )
	{
        PDDBManagedObjectParameter* match = getParameterByNameFrom((*it)->getParameterName(), firstParameters);
		if ( match == NULL )
		{
			// added
            differences.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::AttributeDifference,
                        PDDBManagedObjectCompareResult::ManagedObjectParameter,
                        PDDBManagedObjectCompareResult::Added,
						match, (*it)
			));
		}
	}

	return differences;
}
