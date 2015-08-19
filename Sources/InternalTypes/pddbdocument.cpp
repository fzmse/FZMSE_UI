#include "InternalTypes/PDDBDocument.h"

using namespace std;
using namespace InternalTypes;
using namespace tinyxml2;

PDDBDocument::PDDBDocument(std::string fileName)
{
    this->doc = XmlWrapper::loadDocument(fileName);

    this->managedObjects = retrieveManagedObjects();
}

PDDBDocument::~PDDBDocument()
{
    for ( std::vector<PDDBManagedObject*>::iterator it = this->managedObjects.begin(); it != this->managedObjects.end(); it ++)
        delete (*it);
    delete doc;
}

std::vector<PDDBManagedObject*> PDDBDocument::getManagedObjects()
{
    return this->managedObjects;
}

tinyxml2::XMLDocument * PDDBDocument::getXMLDocument()
{
    return this->doc;
}

PDDBManagedObject * PDDBDocument::getManagedObjectByClassName(std::string name)
{
    for ( std::vector<PDDBManagedObject*>::iterator it = this->managedObjects.begin(); it != this->managedObjects.end(); it ++)
    {
        if ( (*it)->getClassName() == name )
            return (*it);
    }
}

std::vector<PDDBManagedObject*> PDDBDocument::retrieveManagedObjects()
{
    std::vector<PDDBManagedObject*> results;
    XMLElement * d1FirstElement = XmlReader::getFirstElementWithSpecificNameAndAttribute((XMLElement*)this->doc, "managedObject");
    for( XMLElement* node = d1FirstElement; node != NULL; node = node->NextSiblingElement() )
    {
        PDDBManagedObject * curr = new PDDBManagedObject(node);
        results.push_back(curr);
    }
    return results;
}


inline PDDBManagedObject* getMocByClassNameFrom(std::string className, vector<PDDBManagedObject* > list)
{
    for ( vector<PDDBManagedObject* >::iterator it = list.begin();
        it != list.end(); ++ it )
    {
        if ( (*it)->getClassName() == className )
            return (*it);
    }
    return NULL;
}

std::vector<PDDBManagedObjectCompareResult> PDDBDocument::compareDocuments( PDDBDocument * d1, PDDBDocument * d2 )
{
    vector<PDDBManagedObjectCompareResult> results;

    std::vector<PDDBManagedObject*> d1Mocs = d1->getManagedObjects();
    std::vector<PDDBManagedObject*> d2Mocs = d2->getManagedObjects();

    for ( std::vector<PDDBManagedObject *>::iterator it = d1Mocs.begin(); it != d1Mocs.end(); it ++ )
    {
        PDDBManagedObject* match = getMocByClassNameFrom((*it)->getClassName(), d2Mocs);
        if ( match == NULL )
        {
            // Removed
            results.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::ContentDifference,
                                                             PDDBManagedObjectCompareResult::ManagedObject,
                                                             PDDBManagedObjectCompareResult::Removed,
                                                             (*it), NULL));
        }
        else
        {
            // changed
            vector<PDDBManagedObjectCompareResult> changes = (*it)->compare(match);
            for ( vector<PDDBManagedObjectCompareResult>::iterator itChanges = changes.begin(); itChanges != changes.end(); itChanges ++ )
                results.push_back(*itChanges);
        }
    }


    for ( std::vector<PDDBManagedObject *>::iterator it = d2Mocs.begin(); it != d2Mocs.end(); it ++ )
    {
        PDDBManagedObject * match = getMocByClassNameFrom((*it)->getClassName(), d1Mocs);
        if ( match == NULL )
        {
            // Added
            results.push_back(PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::ContentDifference,
                                                             PDDBManagedObjectCompareResult::ManagedObject,
                                                             PDDBManagedObjectCompareResult::Added,
                                                             NULL, (*it)));
        }
    }
    return results;
}


