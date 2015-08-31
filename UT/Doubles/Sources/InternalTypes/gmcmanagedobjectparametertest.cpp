//#include <gtest/gtest.h>
//#include "TinyXML/tinyxml2.h"

//#include "Xml/XmlWrapper.h"
//#include "Xml/XmlElementReader.h"

//#include "InternalTypes/GMCDocument.h"
//#include "InternalTypes/GMCManagedObjectParameter.h"

//static std::string dir = "D:/Projects/FZMSE/FZMSE/";

//std::shared_ptr<InternalTypes::GMCDocument> gmcPar_1506_02 = std::make_shared<InternalTypes::GMCDocument>(dir + "UT/TestFiles/GMC/gmc150602.xml");

//using namespace tinyxml2;
//using namespace std;

//using namespace InternalTypes;





//TEST(GMCManagedObjectParameter, GetManagedObjectParameterName)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjects()[0];
//    GMCManagedObjectParameter * par = moc->getParameters()[0];
//    ASSERT_EQ( "passUnitList", par->getName() );
//}

//TEST(GMCManagedObjectParameter, GetManagedObjectParameterType_01)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjects()[0];
//    GMCManagedObjectParameter * par = moc->getParameters()[0];
//    ASSERT_EQ( GMCManagedObjectParameter::List, par->getType() );
//}

//TEST(GMCManagedObjectParameter, GetManagedObjectParameterType_02)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjects()[0];
//    GMCManagedObjectParameter * par = moc->getParameters()[1];
//    ASSERT_EQ( GMCManagedObjectParameter::SingleParameter, par->getType() );
//}

//TEST(GMCManagedObjectParameter, GetManagedObjectParameterSingleParameterValue)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjects()[0];
//    GMCManagedObjectParameter * par = moc->getParameters()[1];
//    GMCParameterDefaultValue * val = par->getValue();
//    ASSERT_EQ( "actAldSwUpdate", par->getName() );
//    ASSERT_EQ( GMCManagedObjectParameter::SingleParameter, par->getType());
//    EXPECT_EQ( "false", ((GMCParameterSingleValue*)val)->getValue() );
//}


//TEST(GMCManagedObjectParameter, GetManagedObjectParameterListParameterValue_EmptyList)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjects()[0];
//    GMCManagedObjectParameter * par = moc->getParameters()[0];
//    GMCParameterDefaultValue * val = par->getValue();
//    ASSERT_EQ( "passUnitList", par->getName() );
//    ASSERT_EQ( GMCManagedObjectParameter::List, par->getType());
//    EXPECT_EQ( 0, ((GMCParameterListValue*)val)->getSimpleValues().size() );
//}
//TEST(GMCManagedObjectParameter, GetManagedObjectParameterListParameterValue_Simple)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjectByClassName("INTP");
//    GMCManagedObjectParameter * par = moc->getParameterByName("ntpServers");
//    GMCParameterDefaultValue * val = par->getValue();
//    ASSERT_EQ( GMCManagedObjectParameter::List, par->getType());
//    auto values = ((GMCParameterListValue*)val)->getSimpleValues();
//    EXPECT_EQ( 1, values.size() );
//    EXPECT_EQ( "10.254.206.200", values[0].second );
//}

//TEST(GMCManagedObjectParameter, GetManagedObjectParameterListParameterValue_NotEmptyListComplex01)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjects()[5];
//    GMCManagedObjectParameter * par = moc->getParameters()[0];
//    GMCParameterDefaultValue * val = par->getValue();
//    ASSERT_EQ( GMCManagedObjectParameter::List, par->getType());
//    auto values = ((GMCParameterListValue*)val)->getComplexValues();
//    ASSERT_EQ( 1, values.size() );
//    EXPECT_EQ( 3, values[0].size() );
//}

//TEST(GMCManagedObjectParameter, GetManagedObjectParameterListParameterValue_NotEmptyListComplex02)
//{
//    GMCManagedObject * moc = gmcPar_1506_02->getManagedObjects()[4];
//    GMCManagedObjectParameter * par = moc->getParameters()[0];
//    GMCParameterDefaultValue * val = par->getValue();
//    ASSERT_EQ( GMCManagedObjectParameter::List, par->getType());
//    auto values = ((GMCParameterListValue*)val)->getComplexValues();
//    ASSERT_EQ( 2, values.size() );
//    EXPECT_EQ( 2, values[0].size() );
//}

