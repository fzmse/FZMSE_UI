#pragma once
#include <vector>
#include <InternalTypes/PDDBManagedObjectCompareResult.h>
#include <Xml/XmlElementReader.h>
#include <InternalTypes/PDDBManagedObjectParameter.h>
#include <InternalTypes/PDDBManagedObject.h>

#include "InternalTypes/GMCAction.h"

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
                    case PDDBManagedObjectCompareResult::EnumerationChanged:
                        lineMessage += "Value Enumeration changed, ";
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

inline InternalTypes::PDDBManagedObjectCompareResult getCompareResultById(std::vector<InternalTypes::PDDBManagedObjectCompareResult>  v, int id)
{
    for ( std::vector<InternalTypes::PDDBManagedObjectCompareResult>::iterator it = v.begin(); it != v.end(); it ++ )
        if ( (*it).getId() == id )
            return *it;
}

inline std::string getDifferencesText(std::vector<InternalTypes::GMCAction> d, std::vector<InternalTypes::PDDBManagedObjectCompareResult> cmpResults)
{
    using namespace InternalTypes;

    std::string message = "";

    for ( vector<GMCAction>::iterator it = d.begin(); it != d.end(); it ++ )
    {
        std::string lineMessage = "";
        GMCAction r = *it;
        if ( r.isReaderInteractionRequired() )
            lineMessage += "[!] ";
        switch(r.getActionType())
        {
        case GMCAction::Add:
            if ( r.getChangeScope() == GMCAction::ManagedObjectParameter )
            {

                lineMessage += "Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getItem())->getMocParent() != NULL )
                {
                    lineMessage += ((PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getItem())->getMocParent())->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getItem()->getElement(), "name") ;
                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "add ";
            }
            if ( r.getChangeScope() == PDDBManagedObjectCompareResult::ManagedObject )
            {
                lineMessage += "Managed Object [ ";
                lineMessage += ((PDDBManagedObject*)r.getItem())->getClassName();
                lineMessage += " ]";
                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "add ";
            }

            break;
        case GMCAction::Remove:
            if ( r.getChangeScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )
            {
                lineMessage += "Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getItem())->getMocParent() != NULL )
                {
                    PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getItem())->getMocParent();
                    lineMessage += mocObj->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getItem()->getElement(), "name");
                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "remove";
            }

            if ( r.getChangeScope() == PDDBManagedObjectCompareResult::ManagedObject )
            {
                lineMessage += "Managed Object [ ";
                lineMessage += ((PDDBManagedObject*)r.getItem())->getClassName();
                lineMessage += " ]";
                int lineSize = lineMessage.size();
                for ( int i = 83 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "remove";
            }


            break;
        case GMCAction::Modify:
            {
                lineMessage += "Parameter [";
                if ( ((PDDBManagedObjectParameter*)r.getItem())->getMocParent() != NULL )
                {
                    lineMessage += ((PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getItem())->getMocParent())->getClassName();
                    lineMessage += " -> ";
                }
                lineMessage += XmlElementReader::getAttributeByName(r.getItem()->getElement(), "name");

                lineMessage += "]";

                int lineSize = lineMessage.size();
                for ( int i = 85 - lineSize; i >=0; i -- )
                    lineMessage += " ";

                lineMessage += "modify ";

            }
            break;
        }

        message += lineMessage;
        message += " ";
        message += r.getHelpNote();
        message += '\n';
    }
    return message;
}
