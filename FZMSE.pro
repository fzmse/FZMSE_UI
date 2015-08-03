#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T11:07:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FZMSE
TEMPLATE = app

DEFINES += PROJECT_PATH="\"$PWD\""
DEFINES += BUILD_PATH="\"$OUT_PWD\""

SOURCES += main.cpp \
        mainwindow.cpp \
        Includes/TinyXML/tinyxml2.cpp \
        Sources/Xml/XmlReader.cpp \
        Sources/Xml/XmlElementReader.cpp \
        Sources/Xml/XmlWrapper.cpp \
        Sources/InternalTypes/Attribute.cpp \
        Sources/InternalTypes/AttributeDifference.cpp \
        Sources/InternalTypes/ManagedObjectRelativeElement.cpp \
        Sources/InternalTypes/PBDBManagedObject.cpp \
        Sources/InternalTypes/PBDBManagedObjectCompareResult.cpp \
        Sources/InternalTypes/PBDBManagedObjectParameter.cpp \
        UT/Doubles/Sources/mainTest.cpp \
        UT/Doubles/Sources/Xml/XmlReaderTest.cpp \
        UT/Doubles/Sources/Xml/XmlElementReaderTest.cpp \
        UT/Doubles/Sources/Xml/XmlWrapperTest.cpp \
        UT/Doubles/Sources/InternalTypes/PBDBManagedObjectTest.cpp \
        UT/Doubles/Sources/InternalTypes/PBDBManagedObjectParameterTest.cpp \
        UT/Doubles/Sources/Gui/appGuiTest.cpp \
        Sources/Gui/appGui.cpp

HEADERS  += mainwindow.h \
        Includes/TinyXML/tinyxml2.h \
        Includes/Xml/XmlReader.h \
        Includes/Xml/XmlElementReader.h \
        Includes/Xml/XmlWrapper.h \
        Includes/InternalTypes/Attribute.h \
        Includes/InternalTypes/AttributeDifference.h \
        Includes/InternalTypes/GMCManagedObject.h \
        Includes/InternalTypes/ManagedObjectRelativeElement.h \
        Includes/InternalTypes/PBDBManagedObject.h \
        Includes/InternalTypes/PBDBManagedObjectCompareResult.h \
        Includes/InternalTypes/PBDBManagedObjectParameter.h \
        Includes/Utilities/UtilVector.hpp\
        Includes/Gui/appGui.h


FORMS    += mainwindow.ui

INCLUDEPATH += Includes \


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/C:/Qt/5.5/mingw492_32/lib/ -lgtest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/C:/Qt/5.5/mingw492_32/lib/ -lgtest
else:unix: LIBS += -L$$PWD/C:/Qt/5.5/mingw492_32/lib/ -lgtest

INCLUDEPATH += $$PWD/C:/Qt/5.5/mingw492_32/include/gtest
DEPENDPATH += $$PWD/C:/Qt/5.5/mingw492_32/include/gtest

DISTFILES += \
    UT/TestFiles/GMC/notXML.txt \
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
    UT/TestFiles/PDDB/test_pddb_4_new.xml

RESOURCES += \
    source.qrc

