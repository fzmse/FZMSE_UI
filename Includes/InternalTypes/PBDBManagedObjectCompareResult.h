/*
 * PBDBManagedObjectCompareResult.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once


#include "InternalTypes/ManagedObjectRelativeElement.h"

#include "InternalTypes/AttributeDifference.h"

namespace InternalTypes
{
	class PBDBManagedObjectCompareResult
	{
	public:
		enum DifferenceType
		{
			AttributeDifference,
			ContentDifference,
			ContentAndAttributeDifference
		};

		enum DifferenceScope
		{
			ManagedObject,
			ManagedObjectParameter,
			ManagedObjectParameterProperty
		};

		enum DifferenceOrigin
		{
			Modified,
			Added,
			Removed
		};

		PBDBManagedObjectCompareResult( DifferenceType t, DifferenceScope s, DifferenceOrigin o,
				ManagedObjectRelativeElement * fEl, ManagedObjectRelativeElement * sEl,
				std::vector<InternalTypes::AttributeDifference> attribDiffs =
						std::vector<InternalTypes::AttributeDifference>()
				);

		ManagedObjectRelativeElement * getFirstElement();
		ManagedObjectRelativeElement * getSecondElement();

		DifferenceType getType();
		DifferenceScope getScope();
		DifferenceOrigin getOrigin();

		std::vector<InternalTypes::AttributeDifference> getAttributeDifferences();

	protected:
		DifferenceType type;
		DifferenceScope scope;
		DifferenceOrigin origin;

		std::vector<InternalTypes::AttributeDifference> attribDiffs;

		ManagedObjectRelativeElement * firstElement;
		ManagedObjectRelativeElement * secondElement;

	};
}
