/*
 * PBDBManagedObjectCompareResult.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once


#include "InternalTypes/PBDBManagedObjectCompareResult.h"

using namespace InternalTypes;
using namespace std;

PBDBManagedObjectCompareResult::PBDBManagedObjectCompareResult(
		PBDBManagedObjectCompareResult::DifferenceType t,
		PBDBManagedObjectCompareResult::DifferenceScope s,
		PBDBManagedObjectCompareResult::DifferenceOrigin o,
		ManagedObjectRelativeElement* fEl,
		ManagedObjectRelativeElement* sEl,
		std::vector<InternalTypes::AttributeDifference> attribDiffs)
{
	this->type = t;
	this->scope = s;
	this->origin = o;
	this->firstElement = fEl;
	this->secondElement = sEl;
	this->attribDiffs = attribDiffs;
}

ManagedObjectRelativeElement* PBDBManagedObjectCompareResult::getFirstElement()
{
	return this->firstElement;
}

ManagedObjectRelativeElement* PBDBManagedObjectCompareResult::getSecondElement()
{
	return this->secondElement;
}

PBDBManagedObjectCompareResult::DifferenceType PBDBManagedObjectCompareResult::getType()
{
	return this->type;
}

PBDBManagedObjectCompareResult::DifferenceScope PBDBManagedObjectCompareResult::getScope()
{
	return this->scope;
}

PBDBManagedObjectCompareResult::DifferenceOrigin InternalTypes::PBDBManagedObjectCompareResult::getOrigin()
{
	return this->origin;
}

std::vector<InternalTypes::AttributeDifference>
	InternalTypes::PBDBManagedObjectCompareResult::getAttributeDifferences()
{
	return this->attribDiffs;
}
