#pragma once

#include <vector>

#include "PDDBDefaultValue.h"
#include "PDDBManagedObjectParameter.h"

namespace InternalTypes
{
    class PDDBComplexTypeValue : public PDDBDefaultValue
    {
    public:
        PDDBComplexTypeValue(PDDBManagedObjectParameter * p);
        virtual ~PDDBComplexTypeValue();

        virtual bool isComplexType();

        std::vector<PDDBManagedObjectParameter *> getValueParameters();

        std::vector<PDDBManagedObjectCompareResult> compareTo( PDDBComplexTypeValue * p );

        PDDBManagedObjectParameter * getParentParameter();


    protected:

        std::vector<PDDBManagedObjectParameter *> retrieveValueParameters(PDDBManagedObjectParameter * p);

        std::vector<PDDBManagedObjectParameter *> valueParameters;

        PDDBManagedObjectParameter * parentParameter;


    };
}
