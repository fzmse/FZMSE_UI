/*
 * PDDBManagedObjectTest.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

#include <Xml/XmlElementReader.h>

#include <Xml/XmlWrapper.h>

#include "InternalTypes/PDDBManagedObject.h"

#include "Utilities/UtilPDDBHelper.hpp"

#include "InternalTypes/PDDBDocument.h"

using namespace tinyxml2;
using namespace std;

using namespace InternalTypes;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


TEST( PDDBManagedObject, CreateManagedObject)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

	ASSERT_EQ(true, moc->isValidMocObject());
	delete moc;
	delete doc;
}

TEST( PDDBManagedObject, CreateInvalidManagedObject)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "p")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

	ASSERT_EQ(false, moc->isValidMocObject());
	delete moc;
	delete doc;
}

TEST( PDDBManagedObject, CreateManagedObjectWithParameters)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

	ASSERT_EQ(5, moc->getParameters().size());
	delete moc;
	delete doc;
}

TEST( PDDBManagedObject, CreateManagedObjectWithNoParameters)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_noParams.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

	ASSERT_EQ(0, moc->getParameters().size());
	delete moc;
	delete doc;
}

TEST( PDDBManagedObject, CreateManagedObjectWithEmptyParameter)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_emptyParams.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

	ASSERT_EQ(1, moc->getParameters().size());
	delete moc;
	delete doc;
}


TEST( PDDBManagedObject, GetMocIdParameter)
{
    PDDBDocument * d = new PDDBDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
    ASSERT_EQ( "amlePrId" ,d->getManagedObjects()[0]->getMocIdParameter()->getName());
    delete d;
}

TEST( PDDBManagedObjectCompare, CompareIdentical)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

    vector<PDDBManagedObjectCompareResult> differences = moc->compare(moc);

	ASSERT_EQ(0, differences.size());
	delete moc;
	delete doc;
}

TEST( PDDBManagedObjectCompare, CompareAddNewParameterToMoc)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_newParameter.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

    PDDBManagedObject * mocFirst = new PDDBManagedObject(firstMocElement);
    PDDBManagedObject * mocSecond = new PDDBManagedObject(secondMocElement);

    vector<PDDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
    EXPECT_EQ(PDDBManagedObjectCompareResult::ManagedObjectParameter, differences[0].getScope());
    EXPECT_EQ(PDDBManagedObjectCompareResult::Added, differences[0].getOrigin());

	delete mocFirst;
	delete mocSecond;
	delete docFirst;
	delete docSecond;
}

TEST( PDDBManagedObjectCompare, CompareRemoveParameterFromMoc)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_newParameter.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

    PDDBManagedObject * mocFirst = new PDDBManagedObject(secondMocElement);
    PDDBManagedObject * mocSecond = new PDDBManagedObject(firstMocElement);

    vector<PDDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
    EXPECT_EQ(PDDBManagedObjectCompareResult::ManagedObjectParameter, differences[0].getScope());
    EXPECT_EQ(PDDBManagedObjectCompareResult::Removed, differences[0].getOrigin());

	delete mocFirst;
	delete mocSecond;
	delete docFirst;
	delete docSecond;
}


TEST( PDDBManagedObjectCompare, CompareSpecificMOC)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_old.xml");
    XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

    PDDBManagedObject * mocFirst = new PDDBManagedObject(firstMocElement);
    PDDBManagedObject * mocSecond = new PDDBManagedObject(secondMocElement);

    vector<PDDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

    cout << getDifferencesText(differences) << endl;

    delete mocFirst;
    delete mocSecond;
    delete docFirst;
    delete docSecond;
}




TEST( PDDBDocument, LoadPDDBDocument)
{
    shared_ptr<PDDBDocument> docFirst = make_shared<PDDBDocument>(dir+"UT/TestFiles/PDDB/pddb1506.xml");
}



shared_ptr<PDDBDocument> doc1506_02 = make_shared<PDDBDocument>(dir+"UT/TestFiles/PDDB/pddb1506.xml");
shared_ptr<PDDBDocument> doc1507_01 = make_shared<PDDBDocument>(dir+"UT/TestFiles/PDDB/pddb1507.xml");
shared_ptr<PDDBDocument> doc1507_02 = make_shared<PDDBDocument>(dir+"UT/TestFiles/PDDB/pddb150702.xml");
TEST( PDDBManagedObjectCompare, CompareWholePDDB_NoLoadDocument)
{
    vector<PDDBManagedObjectCompareResult> results = PDDBDocument::compareDocuments(doc1506_02.get(), doc1507_01.get());
    // ~ 40 ms -- Comparing documents ^

    cout << getDifferencesText(results) << endl;

}
