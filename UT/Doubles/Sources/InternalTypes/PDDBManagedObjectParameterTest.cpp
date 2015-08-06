#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

#include <Xml/XmlElementReader.h>

#include <Xml/XmlWrapper.h>

#include "InternalTypes/PDDBManagedObject.h"

using namespace tinyxml2;
using namespace std;

using namespace InternalTypes;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


TEST(PDDBManagedObjectParameter, GetProperties)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_1.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);

	vector<ManagedObjectRelativeElement*> properties = moc->getParameters()[0]->getProperties();
	ASSERT_EQ(14, properties.size());

//	for( vector<ManagedObjectRelativeElement*>::iterator it = properties.begin(); it != properties.end(); ++ it )
//		std::cout << XmlElementReader::getName( (*it)->getElement() ) << std::endl;

	delete moc;
	delete doc;
}


