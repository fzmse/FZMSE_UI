/*
 * PBDBManagedObjectTest.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

#include <Xml/XmlElementReader.h>

#include <Xml/XmlWrapper.h>

#include "InternalTypes/PBDBManagedObject.h"

using namespace tinyxml2;
using namespace std;

using namespace InternalTypes;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


TEST( PBDBManagedObject, CreateManagedObject)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

	PBDBManagedObject * moc = new PBDBManagedObject(mocElement);

	ASSERT_EQ(true, moc->isValidMocObject());
	delete moc;
	delete doc;
}

TEST( PBDBManagedObject, CreateInvalidManagedObject)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "p")[0];

	PBDBManagedObject * moc = new PBDBManagedObject(mocElement);

	ASSERT_EQ(false, moc->isValidMocObject());
	delete moc;
	delete doc;
}

TEST( PBDBManagedObject, CreateManagedObjectWithParameters)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

	PBDBManagedObject * moc = new PBDBManagedObject(mocElement);

	ASSERT_EQ(5, moc->getParameters().size());
	delete moc;
	delete doc;
}

TEST( PBDBManagedObject, CreateManagedObjectWithNoParameters)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_noParams.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

	PBDBManagedObject * moc = new PBDBManagedObject(mocElement);

	ASSERT_EQ(0, moc->getParameters().size());
	delete moc;
	delete doc;
}

TEST( PBDBManagedObject, CreateManagedObjectWithEmptyParameter)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_emptyParams.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

	PBDBManagedObject * moc = new PBDBManagedObject(mocElement);

	ASSERT_EQ(1, moc->getParameters().size());
	delete moc;
	delete doc;
}

TEST( PBDBManagedObjectCompare, CompareIdentical)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

	PBDBManagedObject * moc = new PBDBManagedObject(mocElement);

	vector<PBDBManagedObjectCompareResult> differences = moc->compare(moc);

	ASSERT_EQ(0, differences.size());
	delete moc;
	delete doc;
}

TEST( PBDBManagedObjectCompare, CompareChangeInMocAttributeOnly)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_mocAttribChanged.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

	PBDBManagedObject * mocFirst = new PBDBManagedObject(firstMocElement);
	PBDBManagedObject * mocSecond = new PBDBManagedObject(secondMocElement);

	vector<PBDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
	EXPECT_EQ(PBDBManagedObjectCompareResult::ManagedObject, differences[0].getScope());
	EXPECT_EQ(PBDBManagedObjectCompareResult::AttributeDifference, differences[0].getType());

	EXPECT_EQ(1, differences[0].getAttributeDifferences().size());

	delete mocFirst;
	delete mocSecond;
	delete docFirst;
	delete docSecond;
}

TEST( PBDBManagedObjectCompare, CompareAddNewParameterToMoc)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_newParameter.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

	PBDBManagedObject * mocFirst = new PBDBManagedObject(firstMocElement);
	PBDBManagedObject * mocSecond = new PBDBManagedObject(secondMocElement);

	vector<PBDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
	EXPECT_EQ(PBDBManagedObjectCompareResult::ManagedObjectParameter, differences[0].getScope());
	EXPECT_EQ(PBDBManagedObjectCompareResult::Added, differences[0].getOrigin());

	delete mocFirst;
	delete mocSecond;
	delete docFirst;
	delete docSecond;
}

TEST( PBDBManagedObjectCompare, CompareRemoveParameterFromMoc)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_newParameter.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

	PBDBManagedObject * mocFirst = new PBDBManagedObject(secondMocElement);
	PBDBManagedObject * mocSecond = new PBDBManagedObject(firstMocElement);

	vector<PBDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
	EXPECT_EQ(PBDBManagedObjectCompareResult::ManagedObjectParameter, differences[0].getScope());
	EXPECT_EQ(PBDBManagedObjectCompareResult::Removed, differences[0].getOrigin());

	delete mocFirst;
	delete mocSecond;
	delete docFirst;
	delete docSecond;
}

TEST( PBDBManagedObjectCompare, CompareModifyParameterAttribute)
{
    XMLDocument * docFirst = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
	XMLElement* firstMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docFirst, "managedObject")[0];

    XMLDocument * docSecond = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1_paramAttribChange.xml");
	XMLElement* secondMocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) docSecond, "managedObject")[0];

	PBDBManagedObject * mocFirst = new PBDBManagedObject(firstMocElement);
	PBDBManagedObject * mocSecond = new PBDBManagedObject(secondMocElement);

	vector<PBDBManagedObjectCompareResult> differences = mocFirst->compare(mocSecond);

	ASSERT_EQ(1, differences.size());
	EXPECT_EQ(PBDBManagedObjectCompareResult::ManagedObjectParameter, differences[0].getScope());
	EXPECT_EQ(PBDBManagedObjectCompareResult::AttributeDifference, differences[0].getType());
	EXPECT_EQ(PBDBManagedObjectCompareResult::Modified, differences[0].getOrigin());

	delete mocFirst;
	delete mocSecond;
	delete docFirst;
	delete docSecond;
}

