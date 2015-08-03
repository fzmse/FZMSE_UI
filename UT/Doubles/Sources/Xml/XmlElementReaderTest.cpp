#include <vector>

#include <gtest/gtest.h>
#include <Utilities/UtilVector.hpp>
#include <Xml/XmlElementReader.h>

#include <Xml/XmlWrapper.h>


using namespace tinyxml2;
using namespace std;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";

TEST(XML_ELEMENT_READER, GetElementName)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_2.xml");
	ASSERT_EQ(XML_NO_ERROR, doc->ErrorID());

	EXPECT_EQ("relatedParameters", XmlElementReader::getName(doc->RootElement()));

	delete doc;
}

TEST(XML_ELEMENT_READER, GetAttributes)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_2.xml");
	ASSERT_EQ(XML_NO_ERROR, doc->ErrorID());

	XMLElement * el = doc->RootElement()->FirstChildElement();
	vector< pair<string, string> > attributes = XmlElementReader::getAttributes(el);

	vector< pair< string, string> > expected;
	expected.push_back(pair<string, string>("class", "LNBTS"));
	expected.push_back(pair<string, string>("name", "actAmle"));
	expected.push_back(pair<string, string>("hidden", "false"));
	expected.push_back(pair<string, string>("access", "public"));

	ASSERT_EQ( true, isVectorsIdentical( expected, attributes));

	delete doc;
}

TEST(XML_ELEMENT_READER, GetXml)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_2.xml");
	ASSERT_EQ(XML_NO_ERROR, doc->ErrorID());

	XMLElement * el = doc->RootElement()->FirstChildElement();

	cout << XmlElementReader::getXML(el);

	delete doc;
}

TEST(XML_ELEMENT_READER, GetAttributeThatExistsByName)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_2.xml");
	ASSERT_EQ(XML_NO_ERROR, doc->ErrorID());

	XMLElement * el = doc->RootElement()->FirstChildElement();
	ASSERT_EQ("LNBTS", XmlElementReader::getAttributeByName(el, "class") );

	delete doc;
}

TEST(XML_ELEMENT_READER, GetAttributeThatDoesNotExistByName)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_2.xml");
	ASSERT_EQ(XML_NO_ERROR, doc->ErrorID());

	XMLElement * el = doc->RootElement()->FirstChildElement();
	ASSERT_EQ("", XmlElementReader::getAttributeByName(el, "classx") );

	delete doc;
}
