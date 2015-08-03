/*
 * XmlWrapperTest.cpp
 *
 *  Created on: 22 lip 2015
 *      Author: pkozuch
 */
#include <gtest/gtest.h>

#include <Xml/XmlWrapper.h>

using namespace std;
using namespace tinyxml2;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";

TEST(XML_LOAD_FILE, LoadValidFileThatExists)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_2.xml");
    EXPECT_EQ(tinyxml2::XML_NO_ERROR, doc->ErrorID());
	delete doc;
}

TEST(XML_LOAD_FILE, LoadFileThatDoesNotExist)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT_notExists_/test_pddb_12.xml");
	EXPECT_EQ(tinyxml2::XML_ERROR_FILE_NOT_FOUND, doc->ErrorID());
	delete doc;
}

TEST(XML_LOAD_FILE, LoadFileThatIsNotValidXml)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/resources/notXML.txt");
    EXPECT_EQ(tinyxml2::XML_ERROR_PARSING_TEXT, doc->ErrorID());
	delete doc;
}

