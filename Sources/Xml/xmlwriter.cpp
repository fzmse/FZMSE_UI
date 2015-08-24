
#include "Xml/XmlWriter.h"

using namespace tinyxml2;

tinyxml2::XMLDocument * XmlWriter::createDocumentFrom(tinyxml2::XMLDocument * doc)
{
    tinyxml2::XMLDocument * newDoc = new tinyxml2::XMLDocument();
    std::string docXml = XmlElementReader::getXML((tinyxml2::XMLElement*)doc);
    newDoc->Parse(docXml.c_str());
    return newDoc;
}

tinyxml2::XMLElement * XmlWriter::insertAfter(tinyxml2::XMLElement * element, tinyxml2::XMLNode * n)
{
    XMLElement * parent = (XMLElement *)element->Parent();
    if ( parent == NULL )
        return NULL;
    else
        return (XMLElement*)parent->InsertAfterChild(element, n);
}

tinyxml2::XMLElement * XmlWriter::insertChild(tinyxml2::XMLElement * element, tinyxml2::XMLNode * n)
{
    return (XMLElement*)element->InsertFirstChild(n);
}

bool XmlWriter::save(tinyxml2::XMLDocument * doc, std::string filename)
{
    tinyxml2::XMLError err = doc->SaveFile(filename.c_str());
    if ( err == tinyxml2::XMLError::XML_SUCCESS || err == tinyxml2::XMLError::XML_NO_ERROR )
        return true;
    else
        return false;
}
