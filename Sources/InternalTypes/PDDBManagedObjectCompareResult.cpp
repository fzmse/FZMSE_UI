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

int PDDBManagedObjectCompareResult::idCounter = 0;


PDDBManagedObjectCompareResult::PDDBManagedObjectCompareResult(
        PDDBManagedObjectCompareResult::DifferenceType t,
        PDDBManagedObjectCompareResult::DifferenceScope s,
        PDDBManagedObjectCompareResult::DifferenceOrigin o,
		ManagedObjectRelativeElement* fEl,
		ManagedObjectRelativeElement* sEl,
        std::vector<InternalTypes::AttributeDifference> attribDiffs,
        std::vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement> changes,
        ManagedObjectRelativeElement * fComplexParEl,
        ManagedObjectRelativeElement * sComplexParEl )
{
	this->type = t;
	this->scope = s;
	this->origin = o;
	this->firstElement = fEl;
	this->secondElement = sEl;

    this->firstElementComplexTypeParameter = fComplexParEl;
    this->secondElementComplexTypeParameter = sComplexParEl;

	this->attribDiffs = attribDiffs;

    this->changesInParam = changes;

    this->valid = true;

    this->id = idCounter ++;
}

int PDDBManagedObjectCompareResult::getId()
{
    return this->id;
}

PDDBManagedObjectCompareResult PDDBManagedObjectCompareResult::createParamChanged(ManagedObjectRelativeElement * fEl,
                                                                 ManagedObjectRelativeElement * sEl,
                                                                 std::vector<ChangeInMocParameterElement> changes)
{
    return PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::ContentDifference,
                                          PDDBManagedObjectCompareResult::ManagedObjectParameter,
                                          PDDBManagedObjectCompareResult::Modified,
                                          fEl, sEl,
                                          std::vector<InternalTypes::AttributeDifference>(),
                                          changes
                                          );
}

PDDBManagedObjectCompareResult PDDBManagedObjectCompareResult::createComplexParamCompareResult(PDDBManagedObjectCompareResult::DifferenceOrigin origin,
                                                                                               ManagedObjectRelativeElement * fEl,
                                                                                               ManagedObjectRelativeElement * fComplexParEl,
                                                                                               ManagedObjectRelativeElement * sEl,
                                                                                               ManagedObjectRelativeElement * sComplexParEl,
                                                                                               std::vector<ChangeInMocParameterElement> changes)
{
    return PDDBManagedObjectCompareResult(PDDBManagedObjectCompareResult::ContentDifference,
                                          PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter,
                                          origin,
                                          fEl, sEl,
                                          std::vector<InternalTypes::AttributeDifference>(),
                                          changes,
                                          fComplexParEl,
                                          sComplexParEl
                                          );
}

bool PDDBManagedObjectCompareResult::isValid()
{
    return this->valid;
}

PDDBManagedObjectCompareResult::PDDBManagedObjectCompareResult()
{
    this->valid = false;
}

std::vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement> PDDBManagedObjectCompareResult::getChangesInParameter()
{
    return this->changesInParam;
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


InternalTypes::ManagedObjectRelativeElement * InternalTypes::PDDBManagedObjectCompareResult::getFirstElementComplexTypeParameter()
{
    return this->firstElementComplexTypeParameter;
}

InternalTypes::ManagedObjectRelativeElement * InternalTypes::PDDBManagedObjectCompareResult::getSecondElementComplexTypeParameter()
{
    return this->secondElementComplexTypeParameter;
}

bool InternalTypes::PDDBManagedObjectCompareResult::containsChange( PDDBManagedObjectCompareResult::ChangeInMocParameterElement c )
{
    for ( std::vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement>::iterator it = this->changesInParam.begin();
          it != this->changesInParam.end(); it ++)
    {
        if ( *it == c )
            return true;
    }
    return false;
}

