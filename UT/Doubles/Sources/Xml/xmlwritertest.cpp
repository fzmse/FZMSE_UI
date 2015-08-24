#include <gtest/gtest.h>

#include <vector>

#include <Utilities/UtilVector.hpp>
#include <Xml/XmlReader.h>
#include "TinyXML/tinyxml2.h"
#include <Xml/XmlWrapper.h>
#include <Xml/XmlElementReader.h>
#include "Xml/XmlWriter.h"

using namespace tinyxml2;
using namespace std;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";

TEST(XmlWriter, SaveDocument_01)
{
    auto doc = XmlWrapper::loadDocument(dir + "UT/TestFiles/PDDB/test_pddb_1.xml");

    ASSERT_EQ(true, XmlWriter::save(doc, dir + "UT/TestFiles/PDDB/test_pddb_1_saved.xml") );
}

TEST(XmlWriter, CreateDocumentFromAnother)
{
    XMLDocument *  doc = XmlWrapper::loadDocument(dir + "UT/TestFiles/PDDB/test_pddb_1.xml");
    string expectedDocPrint = XmlElementReader::getXML((XMLElement *)doc);
    auto docCopy = XmlWriter::createDocumentFrom(doc);

    XMLElement * newEl = docCopy->NewElement("myNewElement");
    docCopy->InsertFirstChild(newEl);

    // Check if base doc stays the same
    ASSERT_EQ(expectedDocPrint, XmlElementReader::getXML((XMLElement *)doc));

    ASSERT_EQ(true, XmlWriter::save(docCopy,dir + "UT/TestFiles/PDDB/test_pddb_1_saved_01.xml" ));

    delete doc;
    delete docCopy;
}

TEST(XmlWriter, InsertChildNode_01)
{
    XMLDocument *  doc = XmlWrapper::loadDocument(dir + "UT/TestFiles/PDDB/test_pddb_1.xml");
    string expectedDocPrint = XmlElementReader::getXML((XMLElement *)doc);
    auto docCopy = XmlWriter::createDocumentFrom(doc);

    std::string NEW_ELEM_NAME = "myChildManagedObject";

    XMLNode * newNode = docCopy->NewElement(NEW_ELEM_NAME.c_str());

    XMLElement * insertedElement = XmlWriter::insertChild(XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*)docCopy, "managedObject")[0], newNode);

    // Check if base doc stays the same
    ASSERT_EQ(true, insertedElement != NULL);
    ASSERT_EQ(XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*)docCopy, NEW_ELEM_NAME)[0], insertedElement);

    ASSERT_EQ(true, XmlWriter::save(docCopy, dir + "UT/TestFiles/PDDB/test_pddb_1_saved_02.xml" ));

    delete doc;
    delete docCopy;
}


TEST(XmlWriter, InsertAfterNode_01)
{
    XMLDocument *  doc = XmlWrapper::loadDocument(dir + "UT/TestFiles/PDDB/test_pddb_1.xml");
    string expectedDocPrint = XmlElementReader::getXML((XMLElement *)doc);
    auto docCopy = XmlWriter::createDocumentFrom(doc);

    std::string NEW_ELEM_NAME = "ManagedObjectFriend";

    XMLNode * newNode = docCopy->NewElement(NEW_ELEM_NAME.c_str());

    XMLElement * insertedElement = XmlWriter::insertAfter(XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*)docCopy, "managedObject")[0], newNode);

    // Check if base doc stays the same
    ASSERT_EQ(true, insertedElement != NULL);
    ASSERT_EQ(XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*)docCopy, NEW_ELEM_NAME)[0], insertedElement);

    ASSERT_EQ(true, XmlWriter::save(docCopy, dir + "UT/TestFiles/PDDB/test_pddb_1_saved_03.xml" ));

    delete doc;
    delete docCopy;
}
