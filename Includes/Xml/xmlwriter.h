#pragma once

#include <string>
#include <vector>

#include "TinyXML/tinyxml2.h"

#include "Xml/XmlElementReader.h"

class XmlWriter
{
public:
    static tinyxml2::XMLDocument * createDocumentFrom(tinyxml2::XMLDocument * doc);
    static tinyxml2::XMLElement * insertAfter(tinyxml2::XMLElement * element, tinyxml2::XMLNode * n);
    static tinyxml2::XMLElement * insertChild(tinyxml2::XMLElement * element, tinyxml2::XMLNode * n);
    static bool save(tinyxml2::XMLDocument * doc, std::string filename);

};
