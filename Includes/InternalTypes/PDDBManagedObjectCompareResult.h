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
    class PDDBManagedObjectCompareResult
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
            ManagedObjectParameterComplexTypeParameter
		};

		enum DifferenceOrigin
		{
			Modified,
			Added,
			Removed
		};

        enum ChangeInMocParameterElement
        {
            None,
            Description,
            SimpleTypeValue,
            SimpleTypeValue_RangeChanged,
            ComplexTypeValue,
            SimpleToComplex,
            ComplexToSimple,
            VendorSpecific,
            CreationPriority,
            MoMinOccurs,
            RelatedParameters
        };

        PDDBManagedObjectCompareResult( DifferenceType t, DifferenceScope s, DifferenceOrigin o,
                ManagedObjectRelativeElement * fEl, ManagedObjectRelativeElement * sEl,
				std::vector<InternalTypes::AttributeDifference> attribDiffs =
                    std::vector<InternalTypes::AttributeDifference>(),
                std::vector<ChangeInMocParameterElement> changesInParam =
                    std::vector<ChangeInMocParameterElement>(),
                ManagedObjectRelativeElement * fComplexParEl = NULL,
                ManagedObjectRelativeElement * sComplexParEl = NULL

				);
        PDDBManagedObjectCompareResult();

        static PDDBManagedObjectCompareResult createParamChanged(ManagedObjectRelativeElement * fEl,
                                                                 ManagedObjectRelativeElement * sEl,
                                                                 std::vector<ChangeInMocParameterElement> changes );

        static PDDBManagedObjectCompareResult createComplexParamCompareResult(
                                                                 PDDBManagedObjectCompareResult::DifferenceOrigin origin,
                                                                 ManagedObjectRelativeElement * fEl,
                                                                 ManagedObjectRelativeElement * fComplexParEl,
                                                                 ManagedObjectRelativeElement * sEl,
                                                                 ManagedObjectRelativeElement * sComplexParEl,
                                                                 std::vector<ChangeInMocParameterElement> changes =
                                                                                        std::vector<ChangeInMocParameterElement>());


		ManagedObjectRelativeElement * getFirstElement();
		ManagedObjectRelativeElement * getSecondElement();

        ManagedObjectRelativeElement * getFirstElementComplexTypeParameter();
        ManagedObjectRelativeElement * getSecondElementComplexTypeParameter();

        int getId();

        bool isValid();

		DifferenceType getType();
		DifferenceScope getScope();
		DifferenceOrigin getOrigin();

        bool isReaderInteractionRequired();

		std::vector<InternalTypes::AttributeDifference> getAttributeDifferences();

        std::vector<ChangeInMocParameterElement> getChangesInParameter();

        bool containsChange( ChangeInMocParameterElement c );

	protected:

        static int idCounter;
        int id;

        bool valid;

		DifferenceType type;
		DifferenceScope scope;
		DifferenceOrigin origin;


        std::vector<ChangeInMocParameterElement> changesInParam;
		std::vector<InternalTypes::AttributeDifference> attribDiffs;

        ManagedObjectRelativeElement * firstElement;
        ManagedObjectRelativeElement * firstElementComplexTypeParameter;

        ManagedObjectRelativeElement * secondElement;
        ManagedObjectRelativeElement * secondElementComplexTypeParameter;

        bool requiresReaderInteraction;

	};
}
