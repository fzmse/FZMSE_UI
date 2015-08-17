#pragma once

#include "TinyXML/tinyxml2.h"
#include <vector>
#include <string>

class XmlReader {
protected:
	tinyxml2::XMLElement* currElement;
public:
	XmlReader(tinyxml2::XMLDocument* doc);
	XmlReader(tinyxml2::XMLElement* doc);
	virtual ~XmlReader();
	tinyxml2::XMLElement * getCurrElement();
	std::vector <tinyxml2::XMLElement*> getChildren(std::string tag);
	static std::vector <tinyxml2::XMLElement*> getChildren(tinyxml2::XMLElement* e, std::string tag);
	tinyxml2::XMLElement * getFirstChild(std::string tag);
	void setCurrElement(tinyxml2::XMLElement * newElement);
	void setNextElement();
	void goFirstChild();
    static std::vector<tinyxml2::XMLElement *> getElementsWithSpecificNameAndAttributeFromChildrenLevel(tinyxml2::XMLElement * e,
                                                                                                        std::string name, std::string attributeName = "",
                                                                                                        std::string attributeValue = "");
    static std::vector<tinyxml2::XMLElement *> getElementsWithSpecificNameAndAttribute(tinyxml2::XMLElement * e,
                                                                                       std::string name, std::string attributeName = "",
                                                                                       std::string attributeValue = "");

    static tinyxml2::XMLElement * getFirstElementWithSpecificNameAndAttribute(tinyxml2::XMLElement * e,
                                                                                       std::string name);
	static std::vector<tinyxml2::XMLElement *> getLeaves(tinyxml2::XMLElement * e);
	static tinyxml2::XMLElement* getParent(tinyxml2::XMLElement * e);
};
