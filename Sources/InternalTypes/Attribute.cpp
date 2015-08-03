/*
 * Attribute.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once

#include "InternalTypes/Attribute.h"

using namespace InternalTypes;

InternalTypes::Attribute::Attribute()
{

}

InternalTypes::Attribute::Attribute(std::string key, std::string value)
{
	this->key = key;
	this->first = key;

	this->value = value;
	this->second = value;
}

InternalTypes::Attribute::Attribute(std::pair<std::string, std::string> p)
{
	this->key = p.first;
	this->first = p.first;

	this->value = p.second;
	this->second = p.second;
}

std::string InternalTypes::Attribute::getKey()
{
	return this->key;
}

std::string InternalTypes::Attribute::getValue()
{
	return this->value;
}

void InternalTypes::Attribute::setKey(std::string k)
{
	this->key = k;
	this->first = k;
}

void InternalTypes::Attribute::setValue(std::string v)
{
	this->value = v;
	this->second = v;
}



bool Attribute::operator == (Attribute & b)
{
	return this->key == b.getKey() && this->value == b.getValue();
}
