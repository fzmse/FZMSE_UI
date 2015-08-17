#pragma once
#include <vector>
#include <InternalTypes/PDDBManagedObjectCompareResult.h>
#include <Xml/XmlElementReader.h>
#include <InternalTypes/PDDBManagedObjectParameter.h>
#include <InternalTypes/PDDBManagedObject.h>

inline std::string getDifferencesText(std::vector<InternalTypes::PDDBManagedObjectCompareResult> d)
{
    using namespace InternalTypes;

    std::string message = "";

    for ( vector<PDDBManagedObjectCompareResult>::iterator it = d.begin(); it != d.end(); it ++ )
    {
        std::string lineMessage = "";
        PDDBManagedObjectCompareResult r = *it;
        switch(r.getOrigin())
        {
        case PDDBManagedObjectCompareResult::Added:
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )
            {
                lineMessage += "+ Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getSecondElement())->getMocParent() != NULL )
                {
                    lineMessage += ((PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getSecondElement())->getMocParent())->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getSecondElement()->getElement(), "name") ;
                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "added ";
            }
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObject )
            {
                lineMessage += "+ Managed Object [ ";
                lineMessage += ((PDDBManagedObject*)r.getSecondElement())->getClassName();
                lineMessage += " ]";
                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "added ";
            }
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
            {
                lineMessage += "+ Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getSecondElement())->getMocParent() != NULL )
                {
                    lineMessage += ((PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getSecondElement())->getMocParent())->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getSecondElement()->getElement(), "name") ;
                lineMessage += " -> ";
                lineMessage += XmlElementReader::getAttributeByName(r.getSecondElementComplexTypeParameter()->getElement(), "name") ;
                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "added ";
            }
            break;
        case PDDBManagedObjectCompareResult::Removed:
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )
            {
                lineMessage += "- Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getFirstElement())->getMocParent() != NULL )
                {
                    PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getFirstElement())->getMocParent();
                    lineMessage += mocObj->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getFirstElement()->getElement(), "name");
                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "removed";
            }

            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObject )
            {
                lineMessage += "- Managed Object [ ";
                lineMessage += ((PDDBManagedObject*)r.getFirstElement())->getClassName();
                lineMessage += " ]";
                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "removed ";
            }

            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
            {
                lineMessage += "- Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getFirstElement())->getMocParent() != NULL )
                {
                    PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getFirstElement())->getMocParent();
                    lineMessage += mocObj->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getFirstElement()->getElement(), "name");
                lineMessage += " -> ";
                lineMessage += XmlElementReader::getAttributeByName(r.getFirstElementComplexTypeParameter()->getElement(), "name");
                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "removed";
            }
            break;
        case PDDBManagedObjectCompareResult::Modified:
            {
                lineMessage += "Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getFirstElement())->getMocParent() != NULL )
                {
                    lineMessage += ((PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getFirstElement())->getMocParent())->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getFirstElement()->getElement(), "name");
                if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
                {
                    lineMessage += " -> ";
                    lineMessage += XmlElementReader::getAttributeByName(r.getFirstElementComplexTypeParameter()->getElement(), "name");
                }
                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 85 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "modified: ";

                vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement> changes = r.getChangesInParameter();
                for ( vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement>::iterator itParChange = changes.begin();
                      itParChange != changes.end(); itParChange ++)
                {
                    switch (*itParChange)
                    {
                    case PDDBManagedObjectCompareResult::Description:
                        lineMessage += "Description, ";
                    break;
                    case PDDBManagedObjectCompareResult::SimpleTypeValue:
                        lineMessage += "SimpleType value changed, ";
                    break;
                    case PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged:
                        lineMessage += "Range Changed, ";
                    break;
                    case PDDBManagedObjectCompareResult::VendorSpecific:
                        lineMessage += "Vendor Specific, ";
                    break;
                    case PDDBManagedObjectCompareResult::CreationPriority:
                        lineMessage += "Creation Priority, ";
                    break;
                    case PDDBManagedObjectCompareResult::MoMinOccurs:
                        lineMessage += "Mo MinOccurs, ";
                    break;
                    case PDDBManagedObjectCompareResult::RelatedParameters:
                        lineMessage += "Related Parameters, ";
                    break;
                    case PDDBManagedObjectCompareResult::SimpleToComplex:
                        lineMessage += "SimpleValue changed to ComplexValue, ";
                    break;
                    case PDDBManagedObjectCompareResult::ComplexToSimple:
                        lineMessage += "ComplexValue changed to SimpleValue, ";
                    break;
                    case PDDBManagedObjectCompareResult::ComplexTypeValue:
                        lineMessage += "ComplexType value changed";
                    break;

                    }
                }
            }
            break;
        }
        message += lineMessage;
        message += '\n';
    }
    return message;
}
