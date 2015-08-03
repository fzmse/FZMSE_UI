
/*
 * XmlElementReader.cpp
 *
 *  Created on: 22 lip 2015
 *      Author: pkozuch
 */
#include <Xml/XmlElementReader.h>
#include <iostream>

using namespace tinyxml2;
using namespace std;

std::string XmlElementReader::getName(XMLElement * e)
{
	const char * c = e->Value();
	return c == NULL ? "" : std::string(c);
}

vector< pair<string, string> > XmlElementReader::getAttributes(XMLElement * e)
{
	vector< pair<string, string> > result;
	XMLAttribute * currentAttribute = (XMLAttribute*)e->FirstAttribute();

	while ( currentAttribute != NULL )
	{
		pair<string, string> attr;
		attr.first = currentAttribute->Name();
		attr.second = currentAttribute->Value();
		result.push_back(attr);

		currentAttribute = (XMLAttribute*)currentAttribute->Next();
	}
	return result;
}


string XmlElementReader::getAttributeByName(tinyxml2::XMLElement * e, std::string name)
{
	const char * txtPointer = e->Attribute(name.c_str());
	return txtPointer == NULL ? "" : txtPointer;
}


std::string XmlElementReader::getXML(tinyxml2::XMLElement * e)
{
	XMLPrinter printer;
	e->Accept( &printer );
	const char* xmlcstr = printer.CStr();
	return xmlcstr == NULL ? "" : xmlcstr;
}
