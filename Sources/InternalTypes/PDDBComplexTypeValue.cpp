#include "InternalTypes/PDDBComplexTypeValue.h"

using namespace InternalTypes;
using namespace tinyxml2;

PDDBComplexTypeValue::PDDBComplexTypeValue(PDDBManagedObjectParameter *p)
{
    this->parentParameter = p;

    this->valueParameters = retrieveValueParameters(p);
}

PDDBComplexTypeValue::~PDDBComplexTypeValue()
{
    for ( std::vector<PDDBManagedObjectParameter *>::iterator it = this->valueParameters.begin();
          it != this->valueParameters.end();
          it ++)
    {
        if (*it != NULL)
        {
            delete (*it);
        }
    }
}

bool PDDBComplexTypeValue::isComplexType()
{
    return true;
}

std::vector<PDDBManagedObjectParameter *> PDDBComplexTypeValue::getValueParameters()
{
    return this->valueParameters;
}

std::vector<PDDBManagedObjectParameter *> PDDBComplexTypeValue::retrieveValueParameters(PDDBManagedObjectParameter * p)
{
    std::vector<PDDBManagedObjectParameter *> results;

    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(p->getElement(), "complexType");

    if ( elems.size() == 0 )
        return results;

    XMLElement * complexTypeElement = elems[0];

    std::vector<XMLElement *> pElements = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(complexTypeElement, "p");

    for ( std::vector<XMLElement *>::iterator it = pElements.begin(); it != pElements.end(); it ++ )
    {
        if ( *it != NULL )
        {
            PDDBManagedObjectParameter * par = new PDDBManagedObjectParameter(*it);
            results.push_back(par);
        }
    }

    return results;
}

inline PDDBManagedObjectParameter* getMocParameterByNameFrom(std::string name, std::vector<PDDBManagedObjectParameter* > list)
{
    for ( vector<PDDBManagedObjectParameter* >::iterator it = list.begin();
        it != list.end(); ++ it )
    {
        if ( XmlElementReader::getAttributeByName((*it)->getElement(), "name") == name )
            return (*it);
    }
    return NULL;
}


std::vector<PDDBManagedObjectCompareResult> PDDBComplexTypeValue::compareTo( PDDBComplexTypeValue * p )
{
    std::vector<PDDBManagedObjectCompareResult> results;

    std::vector<PDDBManagedObjectParameter* > p1Params = this->getValueParameters();
    std::vector<PDDBManagedObjectParameter* > p2Params = p->getValueParameters();

    for ( std::vector<PDDBManagedObjectParameter*>::iterator it = p1Params.begin(); it != p1Params.end(); it ++ )
    {
        PDDBManagedObjectParameter* match = getMocParameterByNameFrom((*it)->getName(), p2Params);
        if ( match == NULL )
        {
            // Removed
            results.push_back(PDDBManagedObjectCompareResult::createComplexParamCompareResult( PDDBManagedObjectCompareResult::Removed,
                                                             parentParameter, (*it),
                                                             NULL, NULL ));
        }
        else
        {
            // changed
            vector<PDDBManagedObjectCompareResult> changes = (*it)->compareTo(match);
            for ( vector<PDDBManagedObjectCompareResult>::iterator itChanges = changes.begin(); itChanges != changes.end(); itChanges ++ )
            {
                results.push_back(PDDBManagedObjectCompareResult::createComplexParamCompareResult( PDDBManagedObjectCompareResult::Modified,
                                                                     parentParameter, *it,
                                                                     parentParameter, match,
                                                                     (*itChanges).getChangesInParameter()));
            }
        }
    }


    for ( std::vector<PDDBManagedObjectParameter* >::iterator it = p2Params.begin(); it != p2Params.end(); it ++ )
    {
        PDDBManagedObjectParameter* match = getMocParameterByNameFrom((*it)->getName(), p1Params);
        if ( match == NULL )
        {
            // Added
            results.push_back(PDDBManagedObjectCompareResult::createComplexParamCompareResult( PDDBManagedObjectCompareResult::Added,
                                                             NULL, NULL,
                                                             parentParameter, (*it) ));
        }
    }

    return results;
}


PDDBManagedObjectParameter * PDDBComplexTypeValue::getParentParameter()
{
    return this->parentParameter;
}
