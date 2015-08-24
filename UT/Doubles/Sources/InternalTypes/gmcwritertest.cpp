#include <memory>
#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

#include "Xml/XmlWrapper.h"
#include "Xml/XmlElementReader.h"

#include "Xml/XmlWriter.h"

#include "InternalTypes/GMCDocument.h"

#include "Utilities/UtilPDDBHelper.hpp"

#include "InternalTypes/GMCWriter.h"
static std::string dir = "D:/Projects/FZMSE/FZMSE/";

using namespace InternalTypes;
using namespace tinyxml2;

shared_ptr<PDDBDocument> writer_p_1506 = make_shared<PDDBDocument>(dir + "UT/TestFiles/PDDB/pddb1506.xml");
shared_ptr<PDDBDocument> writer_p_1507 = make_shared<PDDBDocument>(dir + "UT/TestFiles/PDDB/pddb1507.xml");
shared_ptr<PDDBDocument> writer_p_1507_myc = make_shared<PDDBDocument>(dir + "UT/TestFiles/PDDB/pddb1507_mychanges.xml");
shared_ptr<GMCDocument> writer_gmc_1506 = make_shared<GMCDocument>(dir + "UT/TestFiles/GMC/gmc150602.xml");

TEST(GMCWriter, AddParameterToGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::insertParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "myNewParameter", "myNewValue");
    GMCWriter::insertParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "aaaBooleanValue", "false");
    GMCWriter::insertParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "zzzBooleanValue", "true");

    //BFDGRP
    GMCWriter::insertParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjectByClassName("BFDGRP"), "somePar", "true");


    //ASSERT_EQ(1, XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*)gmcCopy->getXMLDocument(), "p", "myNewParameter", "myNewValue").size());
    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved02.xml"));
}



TEST(GMCWriter, RemoveParameterFromGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::removeParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "timeZone" );
    GMCWriter::removeParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "actAldSwUpdate" );
    GMCWriter::removeParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "passUnitList" ); // no action - it's a list

    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved03.xml"));
}

TEST(GMCWriter, ModifyParameterFromGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::modifyParameterSimpleType(gmcCopy, writer_gmc_1506->getManagedObjects()[0]->getParameters()[1], "myNewVal" );

    //ASSERT_EQ(1, XmlReader::getElementsWithSpecificNameAndAttribute(writer_gmc_1506->getManagedObjects()[0]->getElement(), "p", "actAldSwUpdate", "myNewVal").size());
    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved04.xml"));
}


TEST(GMCWriter, ModifyComplexParameterFromGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::modifyParameterComplexType(gmcCopy, writer_gmc_1506->getManagedObjectByClassName("LNBTS")->getParameterByName("amRlcPBTab1"), "dlPollByte", "tysiac_piencset_sto_dziewiecset" );

    //ASSERT_EQ(1, XmlReader::getElementsWithSpecificNameAndAttribute(writer_gmc_1506->getManagedObjects()[0]->getElement(), "p", "actAldSwUpdate", "myNewVal").size());
    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved05.xml"));
}

TEST(GMCWriter, RemoveComplexParameterFromGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::removeParameterComplexType(gmcCopy, writer_gmc_1506->getManagedObjectByClassName("LNBTS")->getParameterByName("amRlcPBTab1"), "dlPollByte" );

    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved06.xml"));
}

TEST(GMCWriter, RemoveMocFromGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::removeMoc(gmcCopy, writer_gmc_1506->getManagedObjectByClassName("LNBTS"));

    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved07.xml"));
}

TEST(GMCWriter, AddMocToGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::insertMoc(gmcCopy, "MYNEWMocClass");
    GMCWriter::insertMoc(gmcCopy, "AAAAAAAFirstMoc");
    GMCWriter::insertMoc(gmcCopy, "ZZZZZZZZLastMoc");
    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved08.xml"));
}

TEST(GMCWriter, AddComplexParameterToGMC)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());

    std::vector<pair< string, string> > values;
    values.push_back(pair<string, string>("parname1", "parvalue1"));
    values.push_back(pair<string, string>("parname2", "parvalue2"));
    values.push_back(pair<string, string>("parname3", "parvalue3"));
    values.push_back(pair<string, string>("parname4", "parvalue4"));

    GMCWriter::insertParameterComplexType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "myNewList", values);

    GMCWriter::insertParameterComplexType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "zzzmyNewList", values);
    GMCWriter::insertParameterComplexType(gmcCopy, writer_gmc_1506->getManagedObjects()[0], "aaamyNewList", values);
    ASSERT_EQ(true, XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved09.xml"));
}

TEST(GMCWriter, ProcessGMCWithoutReaderInteraction)
{
    auto compareResults = PDDBDocument::compareDocuments(writer_p_1506.get(), writer_p_1507_myc.get());
    auto actions = GMCDocument::resolveGMCActions(writer_p_1506.get(), writer_p_1507_myc.get(), writer_gmc_1506.get(), &compareResults);

    GMCDocument * gmcCopy = new GMCDocument(writer_gmc_1506.get());
    GMCWriter::reactToAllWithoutReaderInteraction(gmcCopy, actions);

    XmlWriter::save(gmcCopy->getXMLDocument(), dir + "UT/TestFiles/GMC/gmc150602_saved10.xml");
}
