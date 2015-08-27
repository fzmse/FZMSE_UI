#pragma once

#include <string>
#include <vector>

#include "TinyXML/tinyxml2.h"

#include "PDDBDocument.h"

#include "InternalTypes/GMCManagedObject.h"

#include "InternalTypes/GMCAction.h"

namespace InternalTypes
{
    class GMCDocument
    {
    public:
        GMCDocument(std::string fileName);
        GMCDocument(GMCDocument * from);
        ~GMCDocument();

        void reinitialize();


        std::vector<GMCManagedObject*> getManagedObjects();
        GMCManagedObject * getManagedObjectByClassName(std::string name);
        tinyxml2::XMLDocument * getXMLDocument();

        std::vector<GMCManagedObject * > getMocsByClassName(std::string name);

        static std::pair< std::string, std::string> resolveGMCCompareText(GMCDocument * original, GMCDocument * modified, GMCAction action);
        static std::vector<GMCAction> resolveGMCActions( PDDBDocument * oldPDDB, PDDBDocument * newPDDB, GMCDocument * gmc, std::vector<PDDBManagedObjectCompareResult> * compareResults );

    protected:
        static std::vector<GMCAction> resolveGMCActionsPostProcessing( PDDBDocument * oldPDDB, PDDBDocument * newPDDB, GMCDocument * gmc, std::vector<PDDBManagedObjectCompareResult> * compareResults, std::vector<GMCAction> actionsPost );

        std::vector<GMCManagedObject*> retrieveManagedObjects();

        std::vector<GMCManagedObject*> managedObjects;
        tinyxml2::XMLDocument * doc;
    };
}
