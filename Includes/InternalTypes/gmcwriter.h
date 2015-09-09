#pragma once

#include <chrono>

#include "TinyXML/tinyxml2.h"

#include "GMCAction.h"
#include "GMCDocument.h"

#include "Xml/XmlWriter.h"

#include "Report/ReportEntry.h"

#include "InternalTypes/pddbsimpletypevalue.h"
#include "InternalTypes/PDDBComplexTypeValue.h"

namespace InternalTypes
{
    class GMCWriter
    {
    public:
        static void insertParameterSimpleType(GMCDocument * gmc, GMCManagedObject * moc,
                                              std::string name, std::string value );
        static void insertParameterComplexType(GMCDocument * gmc, GMCManagedObject * moc, std::string name,
                                               std::vector< std::pair< std::string, std::string> > values);

        static tinyxml2::XMLElement * insertMoc(GMCDocument * gmc, std::string className, std::string version, std::string operation, std::string distName);

        static void removeParameterSimpleType(GMCDocument * gmc, GMCManagedObject * moc, std::string name);
        static void removeParameterComplexType(GMCDocument * gmc, GMCManagedObjectParameter * complexPar,
                                               std::string name);
        static void removeMoc(GMCDocument * gmc, GMCManagedObject * moc);

        static void modifyParameterSimpleType(GMCDocument * gmc, GMCManagedObjectParameter * par,
                                              std::string value);
        static void modifyParameterComplexType(GMCDocument * gmc, GMCManagedObjectParameter * complexPar,
                                               std::string name, std::string value);

        static std::vector<ReportEntry> reactToAction(GMCDocument * gmc, GMCAction action);

        static std::vector<ReportEntry> reactToAllIncluded( GMCDocument * gmc, std::vector<GMCAction> actions);
        static std::vector<ReportEntry> reactToAll( GMCDocument * gmc, std::vector<GMCAction> actions);

        static void updateVersionInGmc(GMCDocument * gmc, std::string version, std::string releaseName);
    };
}
