#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

#include <Xml/XmlElementReader.h>

#include <Xml/XmlWrapper.h>

#include "InternalTypes/PDDBManagedObject.h"

#include "InternalTypes/PDDBDocument.h"

using namespace tinyxml2;
using namespace std;

using namespace InternalTypes;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


TEST(PDDBManagedObjectParameter, GetDescription)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

    PDDBManagedObjectParameter * parameter = moc->getParameters()[0];
    cout << parameter->getDescription() << endl;

    delete moc;
    delete doc;
}


TEST(PDDBManagedObjectParameter, GetCreationPriority)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

    PDDBManagedObjectParameter * parameter = moc->getParameters()[0];
    ASSERT_EQ("mandatory", parameter->getCreationPriority());

    delete moc;
    delete doc;
}


TEST(PDDBManagedObjectParameter, GetIsVendorSpecific)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    vector<XMLElement*> results = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "p", "name", "glonassLeapSecondDate");
    XMLElement* mocElement = results[0];
    PDDBManagedObjectParameter * mocParam = new PDDBManagedObjectParameter(mocElement, NULL);

    ASSERT_EQ(false, mocParam->isVendorSpecific());

    delete mocParam;
    delete doc;
}

TEST(PDDBManagedObjectParameter, GetMoMinOccurs)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    vector<XMLElement*> results = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "p", "name", "caDPrId");
    XMLElement* mocElement = results[0];
    PDDBManagedObjectParameter * mocParam = new PDDBManagedObjectParameter(mocElement, NULL);

    ASSERT_EQ("0", mocParam->getMoMinOccurs());

    delete mocParam;
    delete doc;
}

TEST(PDDBManagedObjectParameter, GetRelatedParametersRawXml)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    vector<XMLElement*> results = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "p", "name", "caDPrId");
    XMLElement* mocElement = results[0];
    PDDBManagedObjectParameter * mocParam = new PDDBManagedObjectParameter(mocElement, NULL);

    cout << mocParam->getRelatedParametersRawXml() << endl;

    delete mocParam;
    delete doc;
}

TEST(PDDBManagedObjectParameter, GetMaxOccurs)
{
    PDDBDocument * doc = new PDDBDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");

    ASSERT_EQ("1", doc->getManagedObjects()[0]->getParameters()[0]->getMaxOccurs());
    ASSERT_EQ(false, doc->getManagedObjects()[0]->getParameters()[0]->isList());

    delete doc;
}

TEST(PDDBManagedObjectParameter, GetRelatedParams)
{
    PDDBDocument * doc = new PDDBDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");

    ASSERT_EQ(1, doc->getManagedObjects()[0]->getParameters()[0]->getRelatedParameters().size());

    delete doc;
}

TEST(PDDBManagedObjectParameter, Compare01_DescriptionAndRelatedParametersChanged)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_old.xml");
    XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "p", "name", "btsSyncMode")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "p", "name", "btsSyncMode")[0];

    PDDBManagedObjectParameter * mocParFirst = new PDDBManagedObjectParameter(firstMocElement);
    PDDBManagedObjectParameter * mocParSecond = new PDDBManagedObjectParameter(secondMocElement);

    std::vector<PDDBManagedObjectCompareResult> compareResults = mocParFirst->compareTo(mocParSecond);

    ASSERT_EQ(1, compareResults.size());
    ASSERT_EQ(2, compareResults[0].getChangesInParameter().size());

    // Check if truly Description and RelatedParameters Changed
    EXPECT_EQ(true, vectorContains( compareResults[0].getChangesInParameter(), PDDBManagedObjectCompareResult::Description) );
    EXPECT_EQ(true, vectorContains( compareResults[0].getChangesInParameter(), PDDBManagedObjectCompareResult::RelatedParameters) );


    delete mocParFirst;
    delete mocParSecond;
}


TEST(PDDBManagedObjectParameter, Compare02_NoChanges)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_old.xml");
    XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "p", "name", "holdOverModeUsed")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "p", "name", "holdOverModeUsed")[0];

    PDDBManagedObjectParameter * mocParFirst = new PDDBManagedObjectParameter(firstMocElement);
    PDDBManagedObjectParameter * mocParSecond = new PDDBManagedObjectParameter(secondMocElement);

    std::vector<PDDBManagedObjectCompareResult> compareResults = mocParFirst->compareTo(mocParSecond);

    ASSERT_EQ(0, compareResults.size());

    delete mocParFirst;
    delete mocParSecond;
}

TEST(PDDBManagedObjectParameter, Compare03_DescriptionAndSimpleValueChanged)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/pddb1506.xml");
    XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "p", "name", "x2CfUpRspTmr")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/pddb1507.xml");
    XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "p", "name", "x2CfUpRspTmr")[0];

    PDDBManagedObjectParameter * mocParFirst = new PDDBManagedObjectParameter(firstMocElement);
    PDDBManagedObjectParameter * mocParSecond = new PDDBManagedObjectParameter(secondMocElement);

    std::vector<PDDBManagedObjectCompareResult> compareResults = mocParFirst->compareTo(mocParSecond);

    ASSERT_EQ(1, compareResults.size());
    ASSERT_EQ(2, compareResults[0].getChangesInParameter().size());

    EXPECT_EQ(true, vectorContains( compareResults[0].getChangesInParameter(), PDDBManagedObjectCompareResult::Description) );
    EXPECT_EQ(true, vectorContains( compareResults[0].getChangesInParameter(), PDDBManagedObjectCompareResult::SimpleTypeValue) );

    delete mocParFirst;
    delete mocParSecond;
}

TEST(PDDBManagedObjectParameter, Compare04_RelParAndRangeChanged)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/pddb1506.xml");
    XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "p", "name", "lnMmeId")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/pddb1507.xml");
    XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "p", "name", "lnMmeId")[0];

    PDDBManagedObjectParameter * mocParFirst = new PDDBManagedObjectParameter(firstMocElement);
    PDDBManagedObjectParameter * mocParSecond = new PDDBManagedObjectParameter(secondMocElement);

    std::vector<PDDBManagedObjectCompareResult> compareResults = mocParFirst->compareTo(mocParSecond);

    ASSERT_EQ(1, compareResults.size());
    ASSERT_EQ(2, compareResults[0].getChangesInParameter().size());

    EXPECT_EQ(true, vectorContains( compareResults[0].getChangesInParameter(), PDDBManagedObjectCompareResult::RelatedParameters) );
    EXPECT_EQ(true, vectorContains( compareResults[0].getChangesInParameter(), PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged) );

    delete mocParFirst;
    delete mocParSecond;
}

