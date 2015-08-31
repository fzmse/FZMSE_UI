
//#include <gtest/gtest.h>
//#include "TinyXML/tinyxml2.h"

//#include "InternalTypes/PDDBDefaultValue.h"
//#include "InternalTypes/PDDBSimpleTypeValue.h"
//#include "InternalTypes/PDDBComplexTypeValue.h"
//#include "InternalTypes/PDDBManagedObject.h"

//#include "Xml/XmlWrapper.h"
//#include "Xml/XmlElementReader.h"

//using namespace tinyxml2;
//using namespace std;

//using namespace InternalTypes;

//static std::string dir = "D:/Projects/FZMSE/FZMSE/";


//TEST( PDDBSimpleTypeValue, DefaultValueTypeTest)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

//    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
//    PDDBManagedObjectParameter * testedParam = moc->getParameters()[0];

//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    ASSERT_EQ("boolean", def->getTypeName());
//    delete moc;
//    delete doc;
//}


//TEST( PDDBSimpleTypeValue, GetEnumerations)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

//    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
//    PDDBManagedObjectParameter * testedParam = moc->getParameters()[6];

//    vector<PDDBManagedObjectParameter*> params = moc->getParameters();


//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    ASSERT_EQ("integer", def->getTypeName());

//    vector<pair<string, string> > enums = def->getEnums();
//    ASSERT_EQ(1, enums.size());

//    ASSERT_EQ("1", enums[0].first);
//    ASSERT_EQ("PhaseSync", enums[0].second);
//    delete moc;
//    delete doc;
//}


//TEST( PDDBSimpleTypeValue, GetRange)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

//    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
//    PDDBManagedObjectParameter * testedParam = moc->getParameters()[4];

//    vector<PDDBManagedObjectParameter*> params = moc->getParameters();

//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    ASSERT_EQ("decimal", def->getTypeName());

//    vector<pair<string, string> > range = def->getRange();
//    ASSERT_EQ(1, range.size());

//    ASSERT_EQ("0", range[0].first);
//    ASSERT_EQ("1048575", range[0].second);
//    delete moc;
//    delete doc;
//}

//TEST( PDDBSimpleTypeValue, GetEvaluation)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

//    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
//    PDDBManagedObjectParameter * testedParam = moc->getParameters()[7];

//    vector<PDDBManagedObjectParameter*> params = moc->getParameters();

//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    ASSERT_EQ("decimal", def->getTypeName());

//    string eval = def->getEvaluation();
//    ASSERT_EQ("= UI_value", eval);

//    delete moc;
//    delete doc;
//}

//TEST( PDDBSimpleTypeValue, GetRawDefaultValue)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

//    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
//    PDDBManagedObjectParameter * testedParam = moc->getParameters()[0];

//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    ASSERT_EQ("boolean", def->getTypeName());

//    ASSERT_EQ("false", def->getValue());

//    delete moc;
//    delete doc;
//}

//TEST( PDDBSimpleTypeValue, GetEvaluatedValueFromExpression)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

//    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
//    PDDBManagedObjectParameter * testedParam = moc->getParameters()[14];

//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    ASSERT_EQ("decimal", def->getTypeName());

//    ASSERT_EQ("30", def->getEvaluatedValue());

//    delete moc;
//    delete doc;
//}

//TEST( PDDBSimpleTypeValue, GetEvaluatedValueFromComplexExpression)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_5_paramCustomUIValue.xml");

//    PDDBManagedObjectParameter * testedParam = new PDDBManagedObjectParameter((XMLElement*)doc->FirstChild(), NULL);

//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    cout << def->getEvaluatedValue() << endl;

//    delete testedParam;
//    delete doc;
//}


//TEST( PDDBSimpleTypeValue, GetEvaluatedValueFromEnumeration)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    XMLElement* mocElement = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "managedObject")[0];

//    PDDBManagedObject * moc = new PDDBManagedObject(mocElement);
//    PDDBManagedObjectParameter * testedParam = moc->getParameters()[6];

//    PDDBSimpleTypeValue * def = (PDDBSimpleTypeValue * )testedParam->getPDDBValue();

//    ASSERT_EQ("PhaseSync", def->getEvaluatedValue());

//    delete moc;
//    delete doc;
//}


//TEST( PDDBComplexTypeValue, CheckIfParsedCorrectly)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    vector<XMLElement*> results = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "p", "name", "alToggSuppList");
//    XMLElement* mocElement = results[0];
//    PDDBManagedObjectParameter * mocParam = new PDDBManagedObjectParameter(mocElement, NULL);

//    ASSERT_EQ ( true, mocParam->getPDDBValue()->isComplexType());

//    PDDBComplexTypeValue * v = (PDDBComplexTypeValue* )mocParam->getPDDBValue();

//    ASSERT_EQ(5, v->getValueParameters().size());

//    delete mocParam;
//    delete doc;
//}

//TEST( PDDBComplexTypeValue, CheckIfSubParametersFromComplexTypeAreValid)
//{
//    XMLDocument * doc = XmlWrapper::loadDocument(dir+"UT/TestFiles/PDDB/test_pddb_4_new.xml");
//    vector<XMLElement*> results = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*) doc, "p", "name", "alToggSuppList");
//    XMLElement* mocElement = results[0];
//    PDDBManagedObjectParameter * mocParam = new PDDBManagedObjectParameter(mocElement, NULL);

//    ASSERT_EQ ( true, mocParam->getPDDBValue()->isComplexType());

//    PDDBComplexTypeValue * v = (PDDBComplexTypeValue* )mocParam->getPDDBValue();

//    ASSERT_EQ(5, v->getValueParameters().size());

//    std::vector<PDDBManagedObjectParameter *> subParams = v->getValueParameters();

//    EXPECT_EQ("1", ((PDDBSimpleTypeValue*)subParams[0]->getPDDBValue())->getEvaluatedValue());
//    EXPECT_EQ("1", ((PDDBSimpleTypeValue*)subParams[1]->getPDDBValue())->getEvaluatedValue());
//    EXPECT_EQ("1", ((PDDBSimpleTypeValue*)subParams[2]->getPDDBValue())->getEvaluatedValue());
//    EXPECT_EQ("", ((PDDBSimpleTypeValue*)subParams[3]->getPDDBValue())->getEvaluatedValue()); // not specified
//    EXPECT_EQ("false", ((PDDBSimpleTypeValue*)subParams[4]->getPDDBValue())->getEvaluatedValue());

//    delete mocParam;
//    delete doc;
//}

