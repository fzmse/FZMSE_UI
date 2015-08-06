
#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

#include "InternalTypes/PDDBDefaultValue.h"
#include "InternalTypes/PDDBManagedObject.h"

#include "Xml/XmlWrapper.h"
#include "Xml/XmlElementReader.h"

using namespace tinyxml2;
using namespace std;

using namespace InternalTypes;

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


TEST( PDDBDefaultValue, DefaultValueTypeTest)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
    PDDBManagedObjectParameter * testedParam = moc->getParameters()[0];

    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    ASSERT_EQ("boolean", def.getTypeName());
    delete moc;
    delete doc;
}


TEST( PDDBDefaultValue, GetEnumerations)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
    PDDBManagedObjectParameter * testedParam = moc->getParameters()[6];

    vector<PDDBManagedObjectParameter*> params = moc->getParameters();


    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    ASSERT_EQ("integer", def.getTypeName());

    vector<pair<string, string> > enums = def.getEnums();
    ASSERT_EQ(1, enums.size());

    ASSERT_EQ("1", enums[0].first);
    ASSERT_EQ("PhaseSync", enums[0].second);
    delete moc;
    delete doc;
}


TEST( PDDBDefaultValue, GetRange)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
    PDDBManagedObjectParameter * testedParam = moc->getParameters()[4];

    vector<PDDBManagedObjectParameter*> params = moc->getParameters();

    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    ASSERT_EQ("decimal", def.getTypeName());

    vector<pair<string, string> > range = def.getRange();
    ASSERT_EQ(1, range.size());

    ASSERT_EQ("0", range[0].first);
    ASSERT_EQ("1048575", range[0].second);
    delete moc;
    delete doc;
}

TEST( PDDBDefaultValue, GetEvaluation)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
    PDDBManagedObjectParameter * testedParam = moc->getParameters()[7];

    vector<PDDBManagedObjectParameter*> params = moc->getParameters();

    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    ASSERT_EQ("decimal", def.getTypeName());

    string eval = def.getEvaluation();
    ASSERT_EQ("= UI_value", eval);

    delete moc;
    delete doc;
}

TEST( PDDBDefaultValue, GetRawDefaultValue)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
    PDDBManagedObjectParameter * testedParam = moc->getParameters()[0];

    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    ASSERT_EQ("boolean", def.getTypeName());

    ASSERT_EQ("false", def.getValue());

    delete moc;
    delete doc;
}

TEST( PDDBDefaultValue, GetEvaluatedValueFromExpression)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
    PDDBManagedObjectParameter * testedParam = moc->getParameters()[14];

    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    ASSERT_EQ("decimal", def.getTypeName());

    ASSERT_EQ("30", def.getEvaluatedValue());

    delete moc;
    delete doc;
}

TEST( PDDBDefaultValue, GetEvaluatedValueFromComplexExpression)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_5_paramCustomUIValue.xml");

    PDDBManagedObjectParameter * testedParam = new PDDBManagedObjectParameter((XMLElement*)doc);

    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    cout << def.getEvaluatedValue() << endl;

    delete testedParam;
    delete doc;
}


TEST( PDDBDefaultValue, GetEvaluatedValueFromEnumeration)
{
    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
    PDDBManagedObjectParameter * testedParam = moc->getParameters()[6];

    PDDBDefaultValue def = PDDBDefaultValue(testedParam);

    ASSERT_EQ("PhaseSync", def.getEvaluatedValue());

    delete moc;
    delete doc;
}
