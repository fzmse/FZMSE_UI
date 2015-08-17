#pragma once

#include <string>
#include <vector>

#include "Xml/XmlWrapper.h"

#include "InternalTypes/PDDBManagedObject.h"
#include "InternalTypes/PDDBManagedObjectCompareResult.h"

namespace InternalTypes
{
    class PDDBDocument
    {
    public:
        PDDBDocument(std::string fileName);
        ~PDDBDocument();

        static std::vector<PDDBManagedObjectCompareResult> compareDocuments( PDDBDocument * d1, PDDBDocument * d2 );

        std::vector<PDDBManagedObject*> getManagedObjects();
        PDDBManagedObject * getManagedObjectByClassName(std::string name);
        tinyxml2::XMLDocument * getXMLDocument();



    protected:

        std::vector<PDDBManagedObject*> retrieveManagedObjects();

        std::vector<PDDBManagedObject*> managedObjects;
        tinyxml2::XMLDocument * doc;
    };
}
