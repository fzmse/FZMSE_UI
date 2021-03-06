/*
 * PDDBManagedObjectParameter.cpp
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */
#pragma once

#include "InternalTypes/PDDBManagedObjectParameter.h"
#include "InternalTypes/PDDBSimpleTypeValue.h"
#include "InternalTypes/PDDBComplexTypeValue.h"

using namespace InternalTypes;
using namespace tinyxml2;
using namespace std;

PDDBManagedObjectParameter::PDDBManagedObjectParameter(XMLElement * e, ManagedObjectRelativeElement * parentMoc)
    : ManagedObjectRelativeElement::ManagedObjectRelativeElement(e)
{
    this->pddbValue = NULL;
    this->parentMoc = parentMoc;
    this->validMocParameter = false;
    this->vendorSpecific = false;
    if ( e != NULL )
    {
        this->validMocParameter = true;

        // retrieve name
        this->name = retrieveParameterName();

        // retrieve pddb value
        this->pddbValue = retrieveDefaultValue();

        // retireve description
        this->description = retrieveDescription();

        // retrieve creation priority
        this->creationPriority = retrieveCreationPriority();

        // retrieve isVendorSpecific
        this->vendorSpecific = retrieveIsVendorSpecific();

        // retrieve mo minoccurs
        this->moMinOccurs = retrieveMoMinOccurs();

        // retrieve relatedParameters raw xml
        this->relatedParametersRawXml = retrieveRelatedParametersRawXml();

        // retrieve relatedParameters
        this->relatedParameters = retrieveRelatedParameters();

        // retrieve lte name
        this->lteName = retrieveLteName();

        // retrieve maxOccurs Attribute
        this->maxOccurs = retrieveMaxOccurs();
        this->list = this->maxOccurs.size() > 0 && this->maxOccurs != "1";

        // retrieve pronto
        this->pronto = retrievePronto();
    }
}

PDDBManagedObjectParameter::~PDDBManagedObjectParameter()
{
    if ( this->pddbValue != NULL )
        delete this->pddbValue;
}


ManagedObjectRelativeElement * PDDBManagedObjectParameter::getMocParent()
{
    return this->parentMoc;
}

vector<ManagedObjectRelativeElement* > PDDBManagedObjectParameter::getProperties()
{
	return this->properties;
}

std::string PDDBManagedObjectParameter::retrieveParameterName()
{
	return XmlElementReader::getAttributeByName(this->element, "name");
}


std::string PDDBManagedObjectParameter::getDescription()
{
    return this->description;
}

std::string PDDBManagedObjectParameter::retrieveDescription()
{
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "description");
    if ( elems.size() == 0 )
        return "";

    XMLElement * e = elems[0];
    std::vector<XMLElement *> descLines = XmlReader::getElementsWithSpecificNameAndAttribute(e, "para");

    std::string resultString = "";
    for ( std::vector<XMLElement *>::iterator it = descLines.begin(); it != descLines.end(); it ++ )
    {
        XMLElement * el = (*it);
        if ( it != descLines.begin() )
            resultString += '\n';
        const char * txt = el ->GetText();
        resultString += txt == NULL ? "" : txt ;
    }

    return resultString;
}

std::string PDDBManagedObjectParameter::getCreationPriority()
{
    return this->creationPriority;
}

std::string PDDBManagedObjectParameter::retrieveCreationPriority()
{
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "creation");
    if ( elems.size() == 0 )
        return "";

    XMLElement * e = elems[0];
    return XmlElementReader::getAttributeByName(e, "priority");
}


bool PDDBManagedObjectParameter::isVendorSpecific()
{
    return this->vendorSpecific;
}

bool PDDBManagedObjectParameter::retrieveIsVendorSpecific()
{
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "productData");
    if ( elems.size() == 0 )
        return false;

    for ( std::vector<XMLElement *>::iterator it = elems.begin(); it != elems.end(); it ++ )
    {
        XMLElement * e = *it;
        std::vector<XMLElement *> elemsProperties = XmlReader::getElementsWithSpecificNameAndAttribute(e, "property");
        for ( std::vector<XMLElement *>::iterator itProps = elemsProperties.begin(); itProps != elemsProperties.end(); itProps ++ )
        {
            XMLElement * eProps = *itProps;
            std::string res = XmlElementReader::getAttributeByName(eProps, "name");
            if ( res == "isVendorSpecific" )
            {
                if ( XmlElementReader::getAttributeByName(eProps, "value") == "true" )
                    return true;
            }
        }
    }
    return false;
}


std::string PDDBManagedObjectParameter::getMoMinOccurs()
{
    return this->moMinOccurs;
}

std::string PDDBManagedObjectParameter::retrieveMoMinOccurs()
{
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "productData");
    if ( elems.size() == 0 )
        return "";

    for ( std::vector<XMLElement *>::iterator it = elems.begin(); it != elems.end(); it ++ )
    {
        XMLElement * e = *it;
        std::vector<XMLElement *> elemsProperties = XmlReader::getElementsWithSpecificNameAndAttribute(e, "property");
        for ( std::vector<XMLElement *>::iterator itProps = elemsProperties.begin(); itProps != elemsProperties.end(); itProps ++ )
        {
            XMLElement * eProps = *itProps;
            std::string res = XmlElementReader::getAttributeByName(eProps, "name");
            if ( res == "MO MinOccurs" )
            {
                return XmlElementReader::getAttributeByName(eProps, "value");
            }
        }
    }
    return "";
}


PDDBDefaultValue * PDDBManagedObjectParameter::getPDDBValue()
{
    return this->pddbValue;
}

PDDBDefaultValue * PDDBManagedObjectParameter::retrieveDefaultValue()
{
    // check if simple or complexType
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "simpleType");
    if ( elems.size() > 0 )
        return new PDDBSimpleTypeValue(this);
    else
    {
        std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "complexType");
        if ( elems.size() > 0 )
            return new PDDBComplexTypeValue(this);
    }

    return NULL;
}


std::string PDDBManagedObjectParameter::getRelatedParametersRawXml()
{
    return this->relatedParametersRawXml;
}

std::string PDDBManagedObjectParameter::retrieveRelatedParametersRawXml()
{
    return getPropertyXMLByTagName("relatedParameters");
}

std::string PDDBManagedObjectParameter::getName()
{
    return this->name;
}


bool PDDBManagedObjectParameter::isList()
{
    return this->list;
}

std::string PDDBManagedObjectParameter::getMaxOccurs()
{
    return this->maxOccurs;
}

std::string PDDBManagedObjectParameter::retrieveMaxOccurs()
{
    return XmlElementReader::getAttributeByName(this->element, "maxOccurs");
}

std::string PDDBManagedObjectParameter::getLteName()
{
    return this->lteName;
}

std::string PDDBManagedObjectParameter::retrieveLteName()
{
    //feature
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttribute(this->getElement(), "feature");
    if ( elems.size() > 0 )
    {
        XMLElement * lastElement = elems[elems.size() - 1];
        if ( lastElement != NULL )
        {
            std::string featureName = XmlElementReader::getAttributeByName(lastElement, "name");
            if ( featureName.size() >= 8 )
            {
                std::string newStr = "";
                for ( int i = 0; i <= 8; i ++ )
                {
                    if ( featureName[i] == ' ' )
                        return newStr;
                    newStr += featureName[i];
                }
                return newStr;
            }
        }
    }
    return "";
}


std::vector< std::pair < std::string , std::string > > PDDBManagedObjectParameter::getRelatedParameters()
{
    return this->relatedParameters;
}

std::vector< std::pair < std::string , std::string > > PDDBManagedObjectParameter::retrieveRelatedParameters()
{
    std::vector< std::pair < std::string , std::string > > results;
    if ( this->relatedParametersRawXml != "" )
    {
        std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "relatedParameters");
        if ( elems.size() > 0 )
        {
            auto pRefs = XmlReader::getElementsWithSpecificNameAndAttribute(elems[0], "pRef");
            for ( vector<XMLElement*>::iterator it = pRefs.begin(); it != pRefs.end(); it ++  )
            {
                pair<string, string> relPar;
                relPar.first = XmlElementReader::getAttributeByName(*it, "class");
                relPar.second = XmlElementReader::getAttributeByName(*it, "name");
                results.push_back(relPar);
            }
        }
    }
    return results;
}

// Compares the two MOC Parameters and returns differences
std::vector<PDDBManagedObjectCompareResult> PDDBManagedObjectParameter::compareTo(PDDBManagedObjectParameter * p)
{
    PDDBManagedObjectParameter * p1 = this;
    PDDBManagedObjectParameter * p2 = p;

    std::vector<PDDBManagedObjectCompareResult> results;
    std::vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement> changes;

    // Description changed
    if ( p1->getDescription().compare( p2->getDescription() ) != 0 )
        changes.push_back(PDDBManagedObjectCompareResult::Description);

    // Default Value
    PDDBDefaultValue * p1DefValue = p1->getPDDBValue();
    PDDBDefaultValue * p2DefValue = p2->getPDDBValue();

    if ( p1DefValue != NULL && p2DefValue != NULL )
    {
        // SimpleType changed
        if ( p1DefValue->isComplexType() == false && p2DefValue->isComplexType() == false )
        {
            if ( ((PDDBSimpleTypeValue*)p1DefValue)->getEvaluatedValue().compare( ((PDDBSimpleTypeValue*)p2DefValue)->getEvaluatedValue() ) )
                changes.push_back(PDDBManagedObjectCompareResult::SimpleTypeValue);

            if (  ((PDDBSimpleTypeValue*)p1DefValue)->getEnums().size() != ((PDDBSimpleTypeValue*)p2DefValue)->getEnums().size() )
                changes.push_back(PDDBManagedObjectCompareResult::EnumerationChanged);
            else
            {
                auto firstEnums = ((PDDBSimpleTypeValue*)p1DefValue)->getEnums();
                auto secondEnums = ((PDDBSimpleTypeValue*)p2DefValue)->getEnums();

                for ( int i = 0; i < firstEnums.size(); i ++ )
                    if ( firstEnums[i].first != secondEnums[i].first || firstEnums[i].second != secondEnums[i].second  )
                    {
                        changes.push_back(PDDBManagedObjectCompareResult::EnumerationChanged);
                        break;
                    }
            }

            if ( ((PDDBSimpleTypeValue*)p1DefValue)->getRange().size() > 0 && ((PDDBSimpleTypeValue*)p2DefValue)->getRange().size() > 0 )
            {
                if ( ((PDDBSimpleTypeValue*)p1DefValue)->getRange()[0].first.compare( ((PDDBSimpleTypeValue*)p2DefValue)->getRange()[0].first ) != 0 ||
                     ((PDDBSimpleTypeValue*)p1DefValue)->getRange()[0].second.compare( ((PDDBSimpleTypeValue*)p2DefValue)->getRange()[0].second ) != 0 )
                changes.push_back(PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged);
            }
        }

        // SimpleType -> ComplexType
        if ( p1DefValue->isComplexType() == false && p2DefValue->isComplexType() == true )
            changes.push_back(PDDBManagedObjectCompareResult::SimpleToComplex);

        // ComplexType -> SimpleType
        if ( p1DefValue->isComplexType() == true && p2DefValue->isComplexType() == false )
            changes.push_back(PDDBManagedObjectCompareResult::ComplexToSimple);

        // ComplexType changed
        if ( p1DefValue->isComplexType() == true && p2DefValue->isComplexType() == true )
        {
            std::vector<PDDBManagedObjectCompareResult> complexCompareResults =
                    ((PDDBComplexTypeValue*)p1DefValue)->compareTo( ((PDDBComplexTypeValue*)p2DefValue) ) ;

            for ( std::vector<PDDBManagedObjectCompareResult>::iterator itComplexRes = complexCompareResults.begin();
                    itComplexRes != complexCompareResults.end(); itComplexRes ++)
                results.push_back(*itComplexRes);

    // // UNCOMMENT IF WANT SEPARATE INFO THAT COMPLEX VALUE CHANGED
    //        if ( complexCompareResults.size() > 0 )
    //            changes.push_back(PDDBManagedObjectCompareResult::ComplexTypeValue);
        }

        // Vendor Specific
        if ( p1->isVendorSpecific() != p2->isVendorSpecific() )
            changes.push_back(PDDBManagedObjectCompareResult::VendorSpecific);

        //Creation Priority
        if ( p1->creationPriority.compare( p2->creationPriority) != 0 )
            changes.push_back(PDDBManagedObjectCompareResult::CreationPriority);

        // MOMinOccurs
        if ( p1->moMinOccurs.compare(p2->moMinOccurs) != 0 )
            changes.push_back(PDDBManagedObjectCompareResult::MoMinOccurs);

        // Related parameters
        if ( p1->relatedParametersRawXml.compare(p2->relatedParametersRawXml) != 0 )
            changes.push_back(PDDBManagedObjectCompareResult::RelatedParameters);


        // Pack all simple changes in one result
        if ( changes.size() > 0 )
            results.push_back(PDDBManagedObjectCompareResult::createParamChanged(p1, p2, changes));
    }
    return results;
}

std::string PDDBManagedObjectParameter::getPropertyXMLByTagName(std::string tag)
{
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), tag);
    if ( elems.size() > 0 )
    {
        return XmlElementReader::getXML(elems[0]);
    }

    return "";
}

std::string PDDBManagedObjectParameter::getPronto()
{
    return this->pronto;
}


// returns index of the end of allowed sequence or -1 if not found
inline int isStartOfSequence(string s, int index)
{
    const std::vector<std::string> stringMatches({ "PR ", "NA", "CN", "CR", "CRL", "PR", "LTE", "LBT" });
    int sLen = s.length();
    for ( vector<string>::const_iterator it  = stringMatches.begin(); it != stringMatches.end(); it ++)
    {
        string sMatch = *it;
        if ( sLen > (index + sMatch.length() + 1) )
            if ( s.substr(index, sMatch.length()) == sMatch )
            {
                char nextChar = s[index + sMatch.length()];

                // Check if there are numbers or capital letters following the sequence
                if ( ( nextChar >= 'A' && nextChar <= 'Z' ) || ( nextChar >= '0' && nextChar <= '9' ) )
                    return index + sMatch.length();
            }
    }
    return -1;
}

inline bool isNotEndOfProntoChar(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || ( c >= 'a' && c <= 'z' ) || c == '-' ;
}

std::string PDDBManagedObjectParameter::retrievePronto()
{
    std::vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(this->getElement(), "history");
    if ( elems.size() > 0 )
    {
        XMLElement * e = elems[0];
        std::vector<XMLElement *> paraElems = XmlReader::getElementsWithSpecificNameAndAttribute(e, "para");
        if ( paraElems.size() > 0 )
        {
            XMLElement * elPara = paraElems[ paraElems.size() - 1 ];
            std::string paraText = "";
            auto s = elPara->GetText();
            if ( s != NULL )
                paraText = s;
            std::string resultString = "";

            bool isFetchingSequence = false;
            for ( int i = 0; i < paraText.length(); i ++ )
            {
                if ( isFetchingSequence )
                {
                    if ( isNotEndOfProntoChar(paraText[i]) )
                        resultString += paraText[i];
                    else
                        return resultString;
                }
                else
                {
                    int index = isStartOfSequence(paraText, i);
                    if ( index > 0 )
                    {
                        isFetchingSequence = true;
                        string txtToAdd = paraText.substr(i, index - i + 1);
                        resultString += txtToAdd;
                        i = index;
                    }
                }
            }


        }
    }
    return "";
}
