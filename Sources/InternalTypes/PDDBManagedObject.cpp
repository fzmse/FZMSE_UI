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


std::vector<shared_ptr<PDDBManagedObject> > d1Mocs;
std::vector<shared_ptr<PDDBManagedObject> > d2Mocs;

PDDBManagedObject::PDDBManagedObject(XMLElement * e)
	: ManagedObjectRelativeElement::ManagedObjectRelativeElement(e)
{
	this->validMocObject = false;
	if ( e != NULL )
	{
		if ( XmlElementReader::getName(e) == MANAGED_OBJECT_XML_NAME )
		{
			this->validMocObject = true;

            this->className = retrieveClassName(e);

            vector<XMLElement * > pElements = XmlReader::getChildren(e, MANAGED_OBJECT_PARAMETER_XML_NAME);
			for ( vector<XMLElement * >::iterator it = pElements.begin(); it != pElements.end(); ++ it )
			{
				if ( XmlElementReader::getName(*it) == MANAGED_OBJECT_PARAMETER_XML_NAME )
                    this->parameters.push_back( new PDDBManagedObjectParameter(*it, this) );
			}

            this->mocIdParameter = retrieveMocIdParameter();
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
        if ( (*it)->getName() == name )
			return (*it);
	}
	return NULL;
}

vector<PDDBManagedObjectCompareResult> PDDBManagedObject::compare(PDDBManagedObject * moc)
{
    vector<PDDBManagedObjectCompareResult> differences;

        // get parameters difference list
    vector<PDDBManagedObjectParameter*> firstParameters = this->getParameters();
    vector<PDDBManagedObjectParameter*> secondParameters = moc->getParameters();

    for ( vector<PDDBManagedObjectParameter*>::iterator it = firstParameters.begin();
			it != firstParameters.end(); ++ it )
	{
		// find parameter by class
        PDDBManagedObjectParameter* match = getParameterByNameFrom((*it)->getName(), secondParameters);
		if ( match == NULL )
		{
			// removed
            differences.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::ContentDifference,
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

            std::vector<PDDBManagedObjectCompareResult> propertyChanges = (*it)->compareTo(match);
            for ( std::vector<PDDBManagedObjectCompareResult>::iterator itPropertyChanges = propertyChanges.begin();
                  itPropertyChanges != propertyChanges.end(); itPropertyChanges ++)
            {
                differences.push_back(*itPropertyChanges);
            }

		}
	}

    for ( vector<PDDBManagedObjectParameter*>::iterator it = secondParameters.begin();
					it != secondParameters.end(); ++ it )
	{
        PDDBManagedObjectParameter* match = getParameterByNameFrom((*it)->getName(), firstParameters);
		if ( match == NULL )
		{
			// added
            differences.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::ContentDifference,
                        PDDBManagedObjectCompareResult::ManagedObjectParameter,
                        PDDBManagedObjectCompareResult::Added,
						match, (*it)
			));
		}
	}

	return differences;
}

std::string PDDBManagedObject::getClassName()
{
    return this->className;
}

std::string PDDBManagedObject::retrieveClassName(XMLElement * e)
{
    return XmlElementReader::getAttributeByName(e, "class");
}


PDDBManagedObjectParameter * PDDBManagedObject::getMocIdParameter()
{
    return this->mocIdParameter;
}

PDDBManagedObjectParameter * PDDBManagedObject::retrieveMocIdParameter()
{
    std::string expectedParName = this->getClassName();
    std::transform(expectedParName.begin(), expectedParName.end(), expectedParName.begin(), ::tolower);
    expectedParName += "id";
    for ( std::vector<PDDBManagedObjectParameter*>::iterator it = this->parameters.begin(); it != this->parameters.end(); it ++ )
    {
        std::string currName = (*it)->getName();
        std::transform(currName.begin(), currName.end(), currName.begin(), ::tolower);
        if ( currName.compare(expectedParName) == 0 )
            return (*it);
    }
    return NULL;
}
