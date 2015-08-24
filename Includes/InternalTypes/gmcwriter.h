#pragma once

#include "TinyXML/tinyxml2.h"

#include "GMCAction.h"
#include "GMCDocument.h"

#include "Xml/XmlWriter.h"

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

        static tinyxml2::XMLElement * insertMoc(GMCDocument * gmc, std::string className);

        static void removeParameterSimpleType(GMCDocument * gmc, GMCManagedObject * moc, std::string name);
        static void removeParameterComplexType(GMCDocument * gmc, GMCManagedObjectParameter * complexPar,
                                               std::string name);
        static void removeMoc(GMCDocument * gmc, GMCManagedObject * moc);

        static void modifyParameterSimpleType(GMCDocument * gmc, GMCManagedObjectParameter * par,
                                              std::string value);
        static void modifyParameterComplexType(GMCDocument * gmc, GMCManagedObjectParameter * complexPar,
                                               std::string name, std::string value);

        static void reactToAction(GMCDocument * gmc, GMCAction action);

        static void reactToAllWithoutReaderInteraction( GMCDocument * gmc, std::vector<GMCAction> actions);
    };
}