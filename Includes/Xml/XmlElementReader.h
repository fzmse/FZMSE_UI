#pragma once
#include <string>
#include <vector>
#include <TinyXML/tinyxml2.h>
class XmlElementReader
{
public:
	static std::string getName(tinyxml2::XMLElement * e);
	static std::string getXML(tinyxml2::XMLElement * e);
	static std::vector< std::pair<std::string, std::string> > getAttributes(tinyxml2::XMLElement * e);
	static std::string getAttributeByName(tinyxml2::XMLElement * e, std::string name);
};
