/*
 * PBDBManagedObjectParameter.h
 *
 *  Created on: 24 lip 2015
 *      Author: pkozuch
 */

#pragma once

#include <vector>

#include "TinyXML/tinyxml2.h"

#include "PDDBManagedObject.h"

#include "ManagedObjectRelativeElement.h"
#include "PDDBDefaultValue.h"
#include "PDDBManagedObjectCompareResult.h"

namespace InternalTypes
{
    class PDDBManagedObjectParameter : public ManagedObjectRelativeElement
	{
	public:
        PDDBManagedObjectParameter( tinyxml2::XMLElement * pElement, ManagedObjectRelativeElement * parentMoc = NULL );
        virtual ~PDDBManagedObjectParameter();

        ManagedObjectRelativeElement * getMocParent();

        PDDBDefaultValue * getPDDBValue();

		bool isValidMocParameter();
		std::vector<ManagedObjectRelativeElement *> getProperties();

        std::string getDescription();
        std::string getCreationPriority();
        bool isVendorSpecific();
        std::string getMoMinOccurs();
        std::string getRelatedParametersRawXml();
        std::vector< std::pair < std::string , std::string > > getRelatedParameters();
        std::string getName();
        std::string getMaxOccurs();
        std::string getLteName();

        bool isList();

        std::string getPropertyXMLByTagName(std::string tag);

        std::vector<PDDBManagedObjectCompareResult> compareTo(PDDBManagedObjectParameter * p);

	protected:

        ManagedObjectRelativeElement * parentMoc;

        PDDBDefaultValue * pddbValue;

        std::string retrieveParameterName();
        PDDBDefaultValue * retrieveDefaultValue();
        std::string retrieveDescription();
        std::string retrieveCreationPriority();
        bool retrieveIsVendorSpecific();
        std::string retrieveMoMinOccurs();
        std::string retrieveRelatedParametersRawXml();
        std::vector< std::pair < std::string , std::string > > retrieveRelatedParameters();
        std::string retrieveMaxOccurs();
        std::string retrieveLteName();

        std::vector<ManagedObjectRelativeElement* > properties;
		bool validMocParameter;

        std::string description;
        std::string creationPriority;
        std::string moMinOccurs;

        std::string relatedParametersRawXml;
        std::vector< std::pair < std::string , std::string > > relatedParameters;

        std::string name;

        std::string relatedFeaturesRawXml;
        std::string historyRawXml;

        std::string lteName;
        std::string maxOccurs;

        bool list;

        bool vendorSpecific;

	};
}
