#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T11:07:08
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FZMSE
TEMPLATE = app

DEFINES += PROJECT_PATH="\"$PWD\""
DEFINES += BUILD_PATH="\"$OUT_PWD\""

SOURCES += main.cpp \
        Includes/TinyXML/tinyxml2.cpp \
        Sources/Xml/XmlReader.cpp \
        Sources/Xml/XmlElementReader.cpp \
        Sources/Xml/XmlWrapper.cpp \
        Sources/InternalTypes/Attribute.cpp \
        Sources/InternalTypes/AttributeDifference.cpp \
        Sources/InternalTypes/ManagedObjectRelativeElement.cpp \
        UT/Doubles/Sources/mainTest.cpp \
        UT/Doubles/Sources/Xml/XmlReaderTest.cpp \
        UT/Doubles/Sources/Xml/XmlElementReaderTest.cpp \
        UT/Doubles/Sources/Xml/XmlWrapperTest.cpp \
        UT/Doubles/Sources/Gui/appGuiTest.cpp \
        Sources/Gui/appGui.cpp \
    Sources/InternalTypes/PDDBManagedObject.cpp \
    Sources/InternalTypes/PDDBManagedObjectCompareResult.cpp \
    Sources/InternalTypes/PDDBManagedObjectParameter.cpp \
    UT/Doubles/Sources/InternalTypes/PDDBManagedObjectParameterTest.cpp \
    UT/Doubles/Sources/InternalTypes/PDDBManagedObjectTest.cpp \
    UT/Doubles/Sources/InternalTypes/PDDBDefaultValueTest.cpp \
    Includes/ExpressionParser/error.cpp \
    Includes/ExpressionParser/functions.cpp \
    Includes/ExpressionParser/parser.cpp \
    Includes/ExpressionParser/variablelist.cpp \
    UT/Doubles/Sources/Utilities/UtilExpressionParserTest.cpp \
    Sources/InternalTypes/PDDBSimpleTypeValue.cpp \
    Sources/InternalTypes/PDDBComplexTypeValue.cpp \
    Sources/InternalTypes/PDDBDocument.cpp \
    Sources/Gui/resultItemModel.cpp \
    Sources/Gui/resultItem.cpp \
    Sources/Gui/xmlhighlighter.cpp \
    Sources/InternalTypes/GMCDocument.cpp \
    Sources/InternalTypes/GMCManagedObject.cpp \
    UT/Doubles/Sources/InternalTypes/GMCDocumentTest.cpp \
    Sources/InternalTypes/GMCManagedObjectParameter.cpp \
    UT/Doubles/Sources/InternalTypes/GMCManagedObjectParameterTest.cpp \
    Sources/InternalTypes/GMCParameterSingleValue.cpp \
    Sources/InternalTypes/GMCParameterDefaultValue.cpp \
    Sources/InternalTypes/GMCParameterListValue.cpp \
    Sources/InternalTypes/GMCAction.cpp \
    Sources/Gui/gmcresultitem.cpp \
    Sources/Gui/gmcresultitemmodel.cpp \
    Sources/Xml/XmlWriter.cpp \
    UT/Doubles/Sources/Xml/XmlWriterTest.cpp \
    UT/Doubles/Sources/InternalTypes/GMCWriterTest.cpp \
    Sources/InternalTypes/GMCWriter.cpp \
    Sources/Report/ReportUtilities.cpp \
    Sources/Report/ReportEntry.cpp \
    Sources/Gui/helpdialog.cpp


HEADERS  += \
        Includes/TinyXML/tinyxml2.h \
        Includes/Xml/XmlReader.h \
        Includes/Xml/XmlElementReader.h \
        Includes/Xml/XmlWrapper.h \
        Includes/InternalTypes/Attribute.h \
        Includes/InternalTypes/AttributeDifference.h \
        Includes/InternalTypes/GMCManagedObject.h \
        Includes/InternalTypes/ManagedObjectRelativeElement.h \
        Includes/Utilities/UtilVector.hpp\
        Includes/Utilities/UtilExpressionParser.hpp\
        Includes/Gui/appGui.h \
    Includes/InternalTypes/PDDBDefaultValue.h \
    Includes/InternalTypes/PDDBManagedObject.h \
    Includes/InternalTypes/PDDBManagedObjectParameter.h \
    Includes/InternalTypes/PDDBManagedObjectCompareResult.h \
    Includes/ExpressionParser/constants.h \
    Includes/ExpressionParser/error.h \
    Includes/ExpressionParser/functions.h \
    Includes/ExpressionParser/parser.h \
    Includes/ExpressionParser/variablelist.h \
    Includes/Utilities/UtilStrings.hpp \
    Includes/InternalTypes/PDDBSimpleTypeValue.h \
    Includes/InternalTypes/PDDBComplexTypeValue.h \
    Includes/Utilities/UtilPDDBHelper.hpp \
    Includes/InternalTypes/PDDBDocument.h \
    Includes/Gui/resultItemModel.h \
    Includes/Gui/resultItem.h \
    Includes/Gui/xmlhighlighter.h \
    Includes/InternalTypes/GMCDocument.h \
    Includes/InternalTypes/GMCManagedObjectParameter.h \
    Includes/InternalTypes/GMCParameterDefaultValue.h \
    Includes/InternalTypes/GMCParameterSingleValue.h \
    Includes/InternalTypes/GMCParameterListValue.h \
    Includes/InternalTypes/GMCAction.h \
    Includes/Gui/gmcresultitem.h \
    Includes/Gui/gmcresultitemmodel.h \
    Includes/Xml/XmlWriter.h \
    Includes/InternalTypes/GMCWriter.h \
    Includes/Report/ReportUtilities.h \
    Includes/Report/ReportEntry.h \
    Includes/Gui/helpdialog.h

FORMS    += mainwindow.ui

INCLUDEPATH += Includes \


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/C:/Qt/5.5/mingw492_32/lib/ -lgtest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/C:/Qt/5.5/mingw492_32/lib/ -lgtest
else:unix: LIBS += -L$$PWD/C:/Qt/5.5/mingw492_32/lib/ -lgtest

INCLUDEPATH += $$PWD/C:/Qt/5.5/mingw492_32/include/gtest
DEPENDPATH += $$PWD/C:/Qt/5.5/mingw492_32/include/gtest

DISTFILES += \
    UT/TestFiles/GMC/gmc150602.xml \
    UT/TestFiles/PDDB/test_pddb_2.xml \
    UT/TestFiles/PDDB/test_pddb_1_emptyParams.xml \
    UT/TestFiles/PDDB/test_pddb_1_mocAttribChanged.xml \
    UT/TestFiles/PDDB/test_pddb_1_newParameter.xml \
    UT/TestFiles/PDDB/test_pddb_1_noParams.xml \
    UT/TestFiles/PDDB/test_pddb_1_paramAttribChange.xml \
    UT/TestFiles/PDDB/test_pddb_3.xml \
    UT/TestFiles/resources/out/test7.xml \
    UT/TestFiles/resources/out/compact.xml \
    UT/TestFiles/resources/out/pretty.xml \
    UT/TestFiles/resources/out/utf8testout.xml \
    UT/TestFiles/resources/out/dreamout.xml \
    UT/TestFiles/resources/out/textfile.txt \
    UT/TestFiles/resources/bomtest.xml \
    UT/TestFiles/resources/notXML.txt \
    UT/TestFiles/PDDB/test_pddb_1.xml \
    UT/TestFiles/PDDB/test_pddb_4_old.xml \
    UT/TestFiles/PDDB/test_pddb_4_new.xml \
    Includes/Utilities/UtilExpressionParser \
    UT/TestFiles/PDDB/test_pddb_5_paramCustomUIValue.xml \
    UT/TestFiles/PDDB/pddb1506.xml \
    UT/TestFiles/PDDB/pddb1507.xml \
    UT/TestFiles/PDDB/pddb150702.xml \
    UT/TestFiles/PDDB/pddb1507_mychanges.xml \
    UT/TestFiles/PDDB/FL161506.xml \
    UT/TestFiles/PDDB/FL161507.xml \
    Includes/Gui/lineTextEdit \
    UT/TestFiles/PDDB/pddb1507_mychanges.xml \
    UT/TestFiles/PDDB/f13.xml

RESOURCES += appresources.qrc
