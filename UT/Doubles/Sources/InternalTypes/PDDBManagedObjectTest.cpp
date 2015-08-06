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

TEST( PDDBManagedObjectCompare, CompareChangeInMocAttributeOnly)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_mocAttribChanged.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

    PDDBManagedObject * mocFirst = new PDDBManagedObject(firstMocElement);
    PDDBManagedObject * mocSecond = new PDDBManagedObject(secondMocElement);

    vector<PDDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
    EXPECT_EQ(PDDBManagedObjectCompareResult::ManagedObject, differences[0].getScope());
    EXPECT_EQ(PDDBManagedObjectCompareResult::AttributeDifference, differences[0].getType());

	EXPECT_EQ(1, differences[0].getAttributeDifferences().size());
	delete mocFirst;
	delete mocSecond;
	delete docFirst;
	delete docSecond;
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

TEST( PDDBManagedObjectCompare, CompareModifyParameterAttribute)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_paramAttribChange.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

    PDDBManagedObject * mocFirst = new PDDBManagedObject(firstMocElement);
    PDDBManagedObject * mocSecond = new PDDBManagedObject(secondMocElement);

    vector<PDDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
    EXPECT_EQ(PDDBManagedObjectCompareResult::ManagedObjectParameter, differences[0].getScope());
    EXPECT_EQ(PDDBManagedObjectCompareResult::AttributeDifference, differences[0].getType());
    EXPECT_EQ(PDDBManagedObjectCompareResult::Modified, differences[0].getOrigin());

    cout << XmlElementReader::getName( differences[0].getSecondElement()->getElement() ) << endl;


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

    ASSERT_EQ(9, differences.size());
    EXPECT_EQ(PDDBManagedObjectCompareResult::ManagedObjectParameter, differences[0].getScope());
    EXPECT_EQ(PDDBManagedObjectCompareResult::AttributeDifference, differences[0].getType());
    EXPECT_EQ(PDDBManagedObjectCompareResult::Removed, differences[0].getOrigin());

    delete mocFirst;
    delete mocSecond;
    delete docFirst;
    delete docSecond;
}

