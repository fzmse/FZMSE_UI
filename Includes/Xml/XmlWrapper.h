/*
 * XmlParser.h
 *
 *  Created on: 22 lip 2015
 *      Author: pkozuch
 */
#pragma once

#include <memory>
#include <tr1/memory>
#include <TinyXML/tinyxml2.h>

#include <string>



class XmlWrapper
{
public:
	XmlWrapper();
	virtual ~XmlWrapper();
	static tinyxml2::XMLDocument * loadDocument(std::string filename);
};
