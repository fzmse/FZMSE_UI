#pragma once

#include <string>
#include <vector>

#include "Xml/XmlWrapper.h"

#include "InternalTypes/PDDBManagedObject.h"
#include "InternalTypes/PDDBManagedObjectCompareResult.h"

#include "InternalTypes/PDDBComplexTypeValue.h"

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

        std::string getReleaseName();
        std::string getReleaseVersion();



    protected:

        std::vector<PDDBManagedObject*> retrieveManagedObjects();
        std::string retrieveReleaseName();
        std::string retrieveReleaseVersion(std::string rel);

        std::vector<PDDBManagedObject*> managedObjects;
        tinyxml2::XMLDocument * doc;

        std::string releaseName;
        std::string releaseVersion;
    };
}
