/*
 * PDDBManagedObjectCompareResult.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once


#include "InternalTypes/PDDBManagedObjectCompareResult.h"

using namespace InternalTypes;
using namespace std;

PDDBManagedObjectCompareResult::PDDBManagedObjectCompareResult(
        PDDBManagedObjectCompareResult::DifferenceType t,
        PDDBManagedObjectCompareResult::DifferenceScope s,
        PDDBManagedObjectCompareResult::DifferenceOrigin o,
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

ManagedObjectRelativeElement* PDDBManagedObjectCompareResult::getFirstElement()
{
	return this->firstElement;
}

ManagedObjectRelativeElement* PDDBManagedObjectCompareResult::getSecondElement()
{
	return this->secondElement;
}

PDDBManagedObjectCompareResult::DifferenceType PDDBManagedObjectCompareResult::getType()
{
	return this->type;
}

PDDBManagedObjectCompareResult::DifferenceScope PDDBManagedObjectCompareResult::getScope()
{
	return this->scope;
}

PDDBManagedObjectCompareResult::DifferenceOrigin InternalTypes::PDDBManagedObjectCompareResult::getOrigin()
{
	return this->origin;
}

std::vector<InternalTypes::AttributeDifference>
    InternalTypes::PDDBManagedObjectCompareResult::getAttributeDifferences()
{
	return this->attribDiffs;
}
