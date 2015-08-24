#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

#include "Xml/XmlWrapper.h"
#include "Xml/XmlElementReader.h"

#include "InternalTypes/GMCDocument.h"

#include "Utilities/UtilPDDBHelper.hpp"

static std::string dir = "D:/Projects/FZMSE/FZMSE/";

std::shared_ptr<InternalTypes::GMCDocument> gmc_1506_02 = std::make_shared<InternalTypes::GMCDocument>(dir + "UT/TestFiles/GMC/gmc150602.xml");
std::shared_ptr<InternalTypes::PDDBDocument> pddb_1506 = std::make_shared<InternalTypes::PDDBDocument>(dir + "UT/TestFiles/PDDB/pddb1506.xml");
std::shared_ptr<InternalTypes::PDDBDocument> pddbfl16_1506 = std::make_shared<InternalTypes::PDDBDocument>(dir + "UT/TestFiles/PDDB/FL161506.xml");
std::shared_ptr<InternalTypes::PDDBDocument> pddbfl16_1507 = std::make_shared<InternalTypes::PDDBDocument>(dir + "UT/TestFiles/PDDB/FL161507.xml");
std::shared_ptr<InternalTypes::PDDBDocument> pddb_1507 = std::make_shared<InternalTypes::PDDBDocument>(dir + "UT/TestFiles/PDDB/pddb1507.xml");
std::shared_ptr<InternalTypes::PDDBDocument> pddb_1507_mychanges = std::make_shared<InternalTypes::PDDBDocument>(dir + "UT/TestFiles/PDDB/pddb1507_mychanges.xml");
using namespace tinyxml2;
using namespace std;

using namespace InternalTypes;


TEST(GMCDocument, OpenDocument)
{
    ASSERT_EQ( true, gmc_1506_02->getXMLDocument() > 0 );
}

TEST(GMCDocument, GetManagedObjectsCount)
{
    ASSERT_EQ( 55, gmc_1506_02->getManagedObjects().size() );
}

TEST(GMCDocument, GetManagedObjectClassName)
{
    GMCManagedObject * moc = gmc_1506_02->getManagedObjects()[0];
    ASSERT_EQ( "MRBTS",  moc->getClassName());
}

TEST(GMCDocument, GetManagedObjectParametersCount)
{
    GMCManagedObject * moc = gmc_1506_02->getManagedObjects()[0];
    ASSERT_EQ( 8,  moc->getParameters().size());
}



TEST(GMCResolveActions, Moc_Add_01)
{
    auto compareResults = PDDBDocument::compareDocuments(pddb_1506.get(), pddb_1507_mychanges.get());
    auto actions = GMCDocument::resolveGMCActions(pddb_1506.get(), pddb_1507_mychanges.get(), gmc_1506_02.get(), &compareResults);

    ASSERT_EQ(true, actions.size() > 0);
}

TEST(GMCResolveActions, Moc_Add_02)
{
    auto compareResults = PDDBDocument::compareDocuments(pddb_1506.get(), pddb_1507.get());
    auto actions = GMCDocument::resolveGMCActions(pddb_1506.get(), pddb_1507.get(), gmc_1506_02.get(), &compareResults);

    cout << getDifferencesText(actions, compareResults);

    ASSERT_EQ(14, actions.size() );


}

//TEST(GMCResolveActions, PDDBCompare_03)
//{
//    auto compareResults = PDDBDocument::compareDocuments(pddbfl16_1506.get(), pddbfl16_1507.get());
//    auto actions = GMCDocument::resolveGMCActions(pddbfl16_1506.get(), pddbfl16_1507.get(), gmc_1506_02.get(), &compareResults);

//    cout << getDifferencesText(actions, compareResults);

//    ASSERT_EQ(14, actions.size() );


//}


TEST(GMCResolveActions, MocPar_Add_01)
{
    auto compareResults = PDDBDocument::compareDocuments(pddb_1506.get(), pddb_1507_mychanges.get());
    auto actions = GMCDocument::resolveGMCActions(pddb_1506.get(), pddb_1507_mychanges.get(), gmc_1506_02.get(), &compareResults);

    ASSERT_EQ(true, actions.size() > 0);
}
