/*
 * AttributeDifference.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#include "InternalTypes/AttributeDifference.h"

using namespace InternalTypes;
using namespace std;

InternalTypes::AttributeDifference::AttributeDifference(
		std::pair<Attribute, Attribute> diff)
{
	this->first = diff.first;
	this->second = diff.second;
}

InternalTypes::AttributeDifference::AttributeDifference(Attribute a, Attribute b)
{
	this->first = a;
	this->second = b;
}

std::vector<AttributeDifference> InternalTypes::AttributeDifference::getDifferentVectorsOfAttributes(
		std::vector<Attribute> a, std::vector<Attribute> b)
{
	std::vector<AttributeDifference > results;

	if ( a.size() != b.size() )
		return results;

	if ( a.size() == 0 )
		return results;

	std::vector<Attribute>::iterator itA = a.begin();
	std::vector<Attribute>::iterator itB = b.begin();

	while( itA != a.end() && itB != b.end())
	{
		if ( ( (*itA) == (*itB))  == false )
			results.push_back(AttributeDifference( *itA, *itB ));

		++ itA;
		++ itB;
	}

	return results;
}
