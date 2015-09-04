#pragma once

#include "TinyXML/tinyxml2.h"

#include "Xml/XmlWrapper.h"

#include "Xml/XmlWriter.h"

#include "InternalTypes/GMCDocument.h"
#include "InternalTypes/GMCManagedObject.h"

#include "InternalTypes/pddbsimpletypevalue.h"

using namespace InternalTypes;
using namespace tinyxml2;
using namespace std;

GMCDocument::GMCDocument(std::string fileName)
{
    this->doc = XmlWrapper::loadDocument(fileName);

    this->managedObjects = retrieveManagedObjects();
}

GMCDocument::GMCDocument(GMCDocument * from)
{
    this->doc = XmlWriter::createDocumentFrom(from->getXMLDocument());

    this->managedObjects = retrieveManagedObjects();
}

void GMCDocument::reinitialize()
{
    this->managedObjects = retrieveManagedObjects();
}

GMCDocument::~GMCDocument()
{
    for ( std::vector<GMCManagedObject*>::iterator it = this->managedObjects.begin(); it != this->managedObjects.end(); it ++)
        delete (*it);
    delete doc;
}

std::vector<GMCManagedObject*> GMCDocument::getManagedObjects()
{
    return this->managedObjects;
}

GMCManagedObject * GMCDocument::getManagedObjectByClassName(std::string name)
{
    for ( std::vector<GMCManagedObject*>::iterator it = this->managedObjects.begin(); it != this->managedObjects.end(); it ++)
    {
        if ( (*it)->getClassName() == name )
            return (*it);
    }
    return NULL;
}


std::vector<GMCManagedObject * > GMCDocument::getMocsByClassName(std::string name)
{
    std::vector<GMCManagedObject * > results;
    for ( std::vector<GMCManagedObject*>::iterator it = this->managedObjects.begin(); it != this->managedObjects.end(); it ++)
    {
        if ( (*it)->getClassName() == name )
            results.push_back( *it );
    }
    return results;
}

std::vector<GMCManagedObject*> GMCDocument::retrieveManagedObjects()
{
    std::vector<GMCManagedObject*> results;
    XMLElement * d1FirstElement = XmlReader::getFirstElementWithSpecificNameAndAttribute((XMLElement*)this->doc, "managedObject");
    for( XMLElement* node = d1FirstElement; node != NULL; node = node->NextSiblingElement() )
    {
        GMCManagedObject * curr = new GMCManagedObject(node);
        results.push_back(curr);
    }
    return results;
}

tinyxml2::XMLDocument * GMCDocument::getXMLDocument()
{
    return this->doc;
}

inline std::vector<PDDBManagedObjectCompareResult> getAllManagedObjectsChanges(std::vector<PDDBManagedObjectCompareResult> * compareResults)
{
    std::vector<PDDBManagedObjectCompareResult> results;
    for ( std::vector<PDDBManagedObjectCompareResult>::iterator it = compareResults->begin(); it != compareResults->end(); it ++ )
    {
        PDDBManagedObjectCompareResult res = *it;
        if ( res.getScope() == PDDBManagedObjectCompareResult::ManagedObject )
            results.push_back(res);
    }
    return results;
}

bool isAnyOfReferencesValid(vector<pair<string, string> > relParams, GMCDocument * gmc)
{
    for (vector<pair<string, string> >::iterator it = relParams.begin(); it != relParams.end(); it ++ )
    {
        if ( (*it).first != "" )
        {
            auto foundMocs = gmc->getMocsByClassName((*it).first);
            for (vector<GMCManagedObject* >::iterator itmocs = foundMocs.begin(); itmocs != foundMocs.end(); itmocs ++ )
            {
                GMCManagedObject * gmcMoc = *itmocs;
                if ( (*it).second != "" )
                {
                    auto foundParam = gmcMoc->getParameterByName((*it).second);
                    if ( foundParam != NULL )
                        return true;
                }
            }
        }
    }
    return false;
}

inline isFoundInGMCAction(vector<pair<string, string> > relParams, std::vector<GMCAction> actions )
{
    for (vector<pair<string, string> >::iterator it = relParams.begin(); it != relParams.end(); it ++ )
    {
        if ( (*it).first != "" )
        {
            for ( vector<GMCAction>::iterator itA = actions.begin(); itA != actions.end(); itA ++ )
            {
                GMCAction a = *itA;
                if ( a.getActionType() == GMCAction::Add )
                {
                    if ( a.getChangeScope() == GMCAction::ManagedObjectParameter )
                    {
                        auto par = ((PDDBManagedObjectParameter*)a.getItem());
                        if ( par->getName() == (*it).second && ( ((PDDBManagedObject*)par->getMocParent())->getClassName() == (*it).first ) )
                            return true;
                    }
                }
            }
        }
    }
    return false;
}


inline std::vector<GMCAction> resolveGMCActionForMocAdd ( PDDBManagedObjectCompareResult r, GMCDocument * gmc, vector<GMCAction> actionsPost, PDDBManagedObject * currMoc = NULL )
{
    vector<GMCAction> actions;

    PDDBManagedObject * currPDDBMoc = currMoc;
    if ( currMoc == NULL )
        currPDDBMoc = (PDDBManagedObject*)r.getSecondElement();
    auto gmcMocs = gmc->getMocsByClassName(currPDDBMoc->getClassName());
    if ( gmcMocs.size() > 0 )
    {
        // There are mocs like these
    }
    else
    {
        // No traces of such moc in gmc
        PDDBManagedObjectParameter * idPar = currPDDBMoc->getMocIdParameter();
        if ( idPar != NULL )
        {
            vector<GMCAction> actionsForParams;
            if ( idPar->getMoMinOccurs() != "" && idPar->getMoMinOccurs() != "0" )
            {
                // Add Moc To GMC

                // Check For parameters add

                vector<PDDBManagedObjectParameter *> pddbMocParams = currPDDBMoc->getParameters();
                for ( vector<PDDBManagedObjectParameter *>::iterator paramsIt = pddbMocParams.begin();
                      paramsIt != pddbMocParams.end(); paramsIt ++)
                {
                    PDDBManagedObjectParameter * currPar = *paramsIt;
                    if ( currPar != idPar )
                    {
                        if ( currPar->getCreationPriority() == "mandatory" && currPar->isVendorSpecific() == false )
                        {


                                actionsForParams.push_back( GMCAction(r, false,
                                                                      currPar,
                                                                      GMCAction::ActionType::Add,
                                                                      GMCAction::ChangeScope::ManagedObjectParameter,
                                                                      r.getId()) );

                        }
                        else
                        {
                            if ( currPar->getCreationPriority() == "optional" && currPar->isVendorSpecific() == false )
                            {
                                bool refsFoundPost = isFoundInGMCAction(currPar->getRelatedParameters(), actionsPost);
                                bool refsFoundStandard = isAnyOfReferencesValid(currPar->getRelatedParameters(), gmc);
                                if ( refsFoundStandard )
                                {
                                    actionsForParams.push_back( GMCAction(r, true,
                                                                          currPar,
                                                                          GMCAction::ActionType::Add,
                                                                          GMCAction::ChangeScope::ManagedObjectParameter,
                                                                          r.getId(), "Related Parameters references found in GMC"));
                                }
                                else
                                {
                                    if ( refsFoundPost )
                                    {
                                        actionsForParams.push_back( GMCAction(r, true,
                                                                              currPar,
                                                                              GMCAction::ActionType::Add,
                                                                              GMCAction::ChangeScope::ManagedObjectParameter,
                                                                              r.getId(), "Related Parameters references found in GMCActions!!!"));
                                    }
                                }
                            }
                        }
                    }

                }
                actions.push_back( GMCAction(r, false, currPDDBMoc, GMCAction::ActionType::Add,
                                             GMCAction::ChangeScope::ManagedObject, r.getId(),
                                             "",
                                             vector<GMCManagedObject*>(),
                                             actionsForParams, NULL, "update") );

            }
            else
            {
                if ( idPar->getMoMinOccurs() == "0" )
                {
                    if ( isAnyOfReferencesValid(idPar->getRelatedParameters(), gmc) )
                    {
                        vector<PDDBManagedObjectParameter *> pddbMocParams = currPDDBMoc->getParameters();
                        for ( vector<PDDBManagedObjectParameter *>::iterator paramsIt = pddbMocParams.begin();
                              paramsIt != pddbMocParams.end(); paramsIt ++)
                        {
                            PDDBManagedObjectParameter * currPar = *paramsIt;
                            if ( currPar != idPar )
                            {
                                if ( currPar->getCreationPriority() == "mandatory" && currPar->isVendorSpecific() == false )
                                {

                                    actionsForParams.push_back( GMCAction(r, false,
                                                                          currPar,
                                                                          GMCAction::ActionType::Add,
                                                                          GMCAction::ChangeScope::ManagedObjectParameter,
                                                                          r.getId()) );
                                }
                                else
                                {
                                    if ( currPar->getCreationPriority() == "optional" && currPar->isVendorSpecific() == false )
                                    {
                                        bool refsFoundPost = isFoundInGMCAction( currPar->getRelatedParameters(), actionsPost);
                                        bool refsFoundStandard = isAnyOfReferencesValid(currPar->getRelatedParameters(), gmc);
                                        if ( refsFoundStandard )
                                        {
                                            actionsForParams.push_back( GMCAction(r, true,
                                                                                  currPar,
                                                                                  GMCAction::ActionType::Add,
                                                                                  GMCAction::ChangeScope::ManagedObjectParameter,
                                                                                  r.getId(), "Related Parameters references found in GMC"));
                                        }
                                        else
                                        {
                                            if ( refsFoundPost )
                                            {
                                                actionsForParams.push_back( GMCAction(r, true,
                                                                                      currPar,
                                                                                      GMCAction::ActionType::Add,
                                                                                      GMCAction::ChangeScope::ManagedObjectParameter,
                                                                                      r.getId(), "Related Parameters references found in GMCActions!!!"));
                                            }
                                        }
                                    }
                                }
                            }

                        }
                        actions.push_back( GMCAction(r, true, currPDDBMoc, GMCAction::ActionType::Add,
                                                     GMCAction::ChangeScope::ManagedObject, r.getId(),
                                                     "Mo Min Occurs = \"0\" and Managed Object Id Parameter's Related Parameters references found in GMC",
                                                     vector<GMCManagedObject*>(),
                                                     actionsForParams) );
                    }
                }
            }


        }
    }
    return actions;
}

inline std::vector<GMCAction> resolveGMCActionForMocParameterAdd ( PDDBManagedObjectCompareResult r, GMCDocument * gmc, vector<GMCAction> actionsPost )
{
    std::vector<GMCAction> actions;
    PDDBManagedObjectParameter * currPar = (PDDBManagedObjectParameter*)r.getSecondElement();
    PDDBManagedObject * currMoc = (PDDBManagedObject*)currPar->getMocParent();
    auto gmcMocs = gmc->getMocsByClassName(currMoc->getClassName());
    if ( gmcMocs.size() > 0 )
    {

        // There are mocs like these
        //if (  )
        if ( currPar->getCreationPriority() == "mandatory" && currPar->isVendorSpecific() == false )
        {

            actions.push_back( GMCAction(r, false, currPar, GMCAction::ActionType::Add,
                                          GMCAction::ChangeScope::ManagedObjectParameter,
                                          r.getId(),"", gmcMocs) );
        }
        else
        {
            if ( currPar->getCreationPriority() == "optional" && currPar->isVendorSpecific() == false )
            {
                bool anyLuck = true;
                bool refsFoundPost = isFoundInGMCAction( currPar->getRelatedParameters(), actionsPost);
                bool refsFoundStandard = isAnyOfReferencesValid(currPar->getRelatedParameters(), gmc);
                if ( refsFoundStandard )
                {
                    actions.push_back( GMCAction(r, true, currPar, GMCAction::ActionType::Add,
                                              GMCAction::ChangeScope::ManagedObjectParameter,
                                              r.getId(),
                                              "Related Parameters references found in GMC",
                                                 gmcMocs));
                    anyLuck = false;
                }
                else
                {
                    if ( refsFoundPost )
                    {
                        actions.push_back( GMCAction(r, true, currPar, GMCAction::ActionType::Add,
                                                  GMCAction::ChangeScope::ManagedObjectParameter,
                                                  r.getId(),
                                                  "Related Parameters references found in GMCActions!!!",
                                                     gmcMocs));
                        anyLuck = false;
                    }
                }

                // Look for complex types
                if ( anyLuck == true )
                {
                    if ( currPar->getPDDBValue()->isComplexType() )
                    {
                        auto childPars = ((PDDBComplexTypeValue*)currPar->getPDDBValue())->getValueParameters();
                        for ( vector<PDDBManagedObjectParameter*>::iterator it = childPars.begin(); it != childPars.end(); it ++ )
                        {
                            PDDBManagedObjectParameter * childP = *it;
                            if ( childP->getCreationPriority() == "mandatory" && childP->isVendorSpecific() == false  )
                            {
                                bool refsFoundPost = isFoundInGMCAction( childP->getRelatedParameters(), actionsPost);
                                if ( refsFoundPost )
                                {
                                    actions.push_back( GMCAction(r, true, currPar, GMCAction::ActionType::Add,
                                                              GMCAction::ChangeScope::ManagedObjectParameter,
                                                              r.getId(),
                                                              "Related Parameters references found in GMCActions!!!",
                                                                 gmcMocs));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return actions;
}

inline std::vector<GMCAction> resolveGMCActionForMocParameterDelete ( PDDBManagedObjectCompareResult r, GMCDocument * gmc )
{
    std::vector<GMCAction> actions;

    PDDBManagedObjectParameter * currPar = (PDDBManagedObjectParameter*)r.getFirstElement();
    PDDBManagedObject * currMoc = (PDDBManagedObject*)currPar->getMocParent();
    auto gmcMocs = gmc->getMocsByClassName(currMoc->getClassName());
    if ( gmcMocs.size() > 0 )
    {
        if ( gmcMocs[0]->getParameterByName(currPar->getName()) != NULL )
        {
            actions.push_back( GMCAction(r, false, currPar, GMCAction::ActionType::Remove,
                                          GMCAction::ChangeScope::ManagedObjectParameter,
                                          r.getId(),"", gmcMocs) );
        }
    }

    return actions;
}

inline std::vector<GMCAction> resolveGMCActionForMocDelete ( PDDBManagedObjectCompareResult r, GMCDocument * gmc )
{
    std::vector<GMCAction> actions;
    PDDBManagedObject * currPDDBMoc = (PDDBManagedObject*)r.getFirstElement();
    auto gmcMocs = gmc->getMocsByClassName(currPDDBMoc->getClassName());
    if ( gmcMocs.size() > 0 )
    {

        actions.push_back( GMCAction(r, false, currPDDBMoc, GMCAction::ActionType::Remove,
                                      GMCAction::ChangeScope::ManagedObject,
                                      r.getId(),"", gmcMocs) );
    }
    return actions;
}

std::vector<GMCAction> GMCDocument::resolveGMCActionsPostProcessing( PDDBDocument * oldPDDB, PDDBDocument * newPDDB, GMCDocument * gmc, std::vector<PDDBManagedObjectCompareResult> * compareResults, std::vector<GMCAction> actionsPost )
{
    std::vector<GMCAction> actions; // ID DESYNCH ?? CHECK TODO

    // Resolve Managed Objects Actions
    //auto changes = compareResults;
    for ( std::vector<PDDBManagedObjectCompareResult>::iterator it = compareResults->begin(); it != compareResults->end(); it ++ )
    {
        PDDBManagedObjectCompareResult r = *it;

        // TODO COMPLEX PARAMETERS!!!!!
        if ( r.getOrigin() == PDDBManagedObjectCompareResult::Added )
        {
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObject )           // ADDED MANAGED OBJECT ~~~~~~~~~~~~
            {
                auto newAction = resolveGMCActionForMocAdd(r, gmc, actionsPost);
                if ( newAction.size() > 0 )
                    actions.push_back(newAction[0]);
            }

            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )  // ADDED MANAGED OBJECT PARAMETER ~~~~~~~~~~~~
            {
                auto newAction = resolveGMCActionForMocParameterAdd(r, gmc, actionsPost);
                if ( newAction.size() > 0 )
                    actions.push_back(newAction[0]);
            }
        }


        if ( r.getOrigin() == PDDBManagedObjectCompareResult::Removed )
        {
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )   // REMOVED MANAGED OBJECT PARAMETER ~~~~~~~~~~~~
            {
                auto newAction = resolveGMCActionForMocParameterDelete(r, gmc);
                if ( newAction.size() > 0 )
                    actions.push_back(newAction[0]);
            }

            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObject )
            {
                auto newAction = resolveGMCActionForMocDelete(r, gmc);
                if ( newAction.size() > 0 )
                    actions.push_back(newAction[0]);
            }
        }

        if ( r.getOrigin() == PDDBManagedObjectCompareResult::Modified )
        {
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )
            {
                PDDBManagedObjectParameter * oldPar = (PDDBManagedObjectParameter*)r.getFirstElement();
                PDDBManagedObjectParameter * newPar = (PDDBManagedObjectParameter*)r.getSecondElement();
                PDDBManagedObject * currMocOld = (PDDBManagedObject*)oldPar->getMocParent();
                PDDBManagedObject * currMocNew = (PDDBManagedObject*)newPar->getMocParent();
                auto gmcMocs = gmc->getMocsByClassName(currMocNew->getClassName());
                if ( gmcMocs.size() > 0 )
                {
                    if ( newPar->isVendorSpecific() == true || newPar->getCreationPriority() == "Value set by the system")
                    {
                        if ( gmcMocs[0]->getParameterByName(oldPar->getName()) != NULL )
                        {
                            actions.push_back( GMCAction(r, false, newPar, GMCAction::ActionType::Remove,
                                                      GMCAction::ChangeScope::ManagedObjectParameter,
                                                      r.getId(),
                                                      "",
                                                         gmcMocs));
                        }
                    }
                    else
                    {
                        // Default values changes

                        if ( r.containsChange(PDDBManagedObjectCompareResult::SimpleTypeValue) && r.getChangesInParameter().size() == 1 )
                        {
                            string oldValue = ((PDDBSimpleTypeValue*)oldPar->getPDDBValue())->getEvaluatedValue();
                            string newValue = ((PDDBSimpleTypeValue*)newPar->getPDDBValue())->getEvaluatedValue();

                            GMCManagedObjectParameter * gmcCurrPar = gmcMocs[0]->getParameterByName(newPar->getName());
                            if ( gmcCurrPar != NULL )
                            {
                                if ( gmcCurrPar->getType() == GMCManagedObjectParameter::SingleParameter)
                                {
                                    auto val = (GMCParameterSingleValue*)gmcCurrPar->getValue();
                                    if ( val != NULL )
                                    {

                                        if ( oldValue == val->getValue() && newValue != val->getValue())
                                        {

                                            actions.push_back( GMCAction(r, r.containsChange(PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged),
                                                                        newPar, GMCAction::ActionType::Modify,
                                                                        GMCAction::ChangeScope::ManagedObjectParameter,
                                                                        r.getId(),
                                                                        r.containsChange(PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged)
                                                                                        ? "Range changed, attention required" : "",
                                                                        gmcMocs, vector<GMCAction>(), oldPar));
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if ( r.containsChange(PDDBManagedObjectCompareResult::VendorSpecific) && newPar->getCreationPriority() == "mandatory"
                                 && newPar->isVendorSpecific() == false )
                            {
                                // Check if in GMC
                                if ( gmcMocs[0]->getParameterByName(oldPar->getName()) == NULL )
                                    // add to gmc
                                        actions.push_back( GMCAction(r, false, newPar, GMCAction::ActionType::Add,
                                                                      GMCAction::ChangeScope::ManagedObjectParameter,
                                                                      r.getId(),"", gmcMocs) );
                            }
                            else
                            {
                                if ( r.containsChange(PDDBManagedObjectCompareResult::CreationPriority) )
                                {
                                    if ( newPar->getCreationPriority() == "mandatory"  )
                                    {
                                        // Check if in GMC
                                        if ( gmcMocs[0]->getParameterByName(oldPar->getName()) == NULL )
                                            // add to gmc
                                                actions.push_back( GMCAction(r, false, newPar, GMCAction::ActionType::Add,
                                                                              GMCAction::ChangeScope::ManagedObjectParameter,
                                                                              r.getId(),"", gmcMocs) );
                                    }
                                    else
                                    {
                                        if ( newPar->getCreationPriority() == "optional" )
                                        {
                                            // check if in GMC
                                            if ( gmcMocs[0]->getParameterByName(newPar->getName()) != NULL )
                                            {
                                                string message = "Creation Priority changed to optional";
                                                bool refsFoundPost = isFoundInGMCAction( newPar->getRelatedParameters(), actionsPost);
                                                bool refsFoundStandard = isAnyOfReferencesValid(newPar->getRelatedParameters(), gmc);
                                                if ( refsFoundStandard )
                                                    message += ", Related Parameters references found in GMC";
                                                else
                                                    if ( refsFoundPost )
                                                        message += ", Related Parameters references found in GMCActions!!!";
                                                actions.push_back( GMCAction(r, true, newPar, GMCAction::ActionType::Remove,
                                                                          GMCAction::ChangeScope::ManagedObjectParameter,
                                                                          r.getId(), message, gmcMocs));
                                            }
                                        }
                                    }
                                }
                                else
                                {

                                    if ( r.containsChange(PDDBManagedObjectCompareResult::RelatedParameters) && r.containsChange(PDDBManagedObjectCompareResult::CreationPriority) )
                                    {
                                        string message = "Related Parameters changed";
                                        if ( r.containsChange(PDDBManagedObjectCompareResult::Description) )
                                            message += ", Description changed";

                                        if ( gmcMocs[0]->getParameterByName(newPar->getName()) != NULL ) // in gmc
                                        {
                                            if ( newPar->getCreationPriority() != "mandatory"  )
                                                actions.push_back( GMCAction(r, true, newPar, GMCAction::ActionType::Remove,
                                                                          GMCAction::ChangeScope::ManagedObjectParameter,
                                                                          r.getId(), message, gmcMocs));
                                        }
                                        else
                                        {
                                            if ( newPar->getCreationPriority() != "mandatory"  )
                                                actions.push_back( GMCAction(r, true, newPar, GMCAction::ActionType::Add,
                                                                          GMCAction::ChangeScope::ManagedObjectParameter,
                                                                          r.getId(), message, gmcMocs));
                                        }
                                    }
                                    else
                                    {
                                        if ( r.containsChange(PDDBManagedObjectCompareResult::SimpleTypeValue) )
                                        {
                                            if ( gmcMocs[0]->getParameterByName(newPar->getName()) != NULL ) // in gmc
                                            {
                                                actions.push_back( GMCAction(r, r.containsChange(PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged),
                                                                            newPar, GMCAction::ActionType::Modify,
                                                                            GMCAction::ChangeScope::ManagedObjectParameter,
                                                                            r.getId(),
                                                                            r.containsChange(PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged)
                                                                                            ? "Range changed, attention required" : "",
                                                                            gmcMocs, vector<GMCAction>(),
                                                                             oldPar));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if ( r.containsChange(PDDBManagedObjectCompareResult::MoMinOccurs) )
                    {
                        PDDBManagedObjectParameter * mocParId = (PDDBManagedObjectParameter*)r.getSecondElement();
                        PDDBManagedObject * mocParIdParentMoc = (PDDBManagedObject*)mocParId->getMocParent();
                        if ( mocParIdParentMoc != NULL )
                        {
                            if ( mocParIdParentMoc->getMocIdParameter() == mocParId )
                            {
                                if ( mocParId->getMoMinOccurs() != "" && mocParId->getMoMinOccurs() != "0" )
                                {
                                    auto newAction = resolveGMCActionForMocAdd(r, gmc, actionsPost, mocParIdParentMoc);
                                    if ( newAction.size() > 0 )
                                        actions.push_back(newAction[0]);
                                }
                            }
                        }
                    }
                }
            }
        }

        // COMPLEX TYPE CHANGED

        if ( r.getOrigin() == PDDBManagedObjectCompareResult::Modified )
        {
            if ( r.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )           // ADDED MANAGED OBJECT ~~~~~~~~~~~~
            {
                PDDBManagedObjectParameter * currPar = (PDDBManagedObjectParameter*)r.getSecondElement(); // complexTypeParameter
                PDDBManagedObject * currMoc = (PDDBManagedObject*)currPar->getMocParent();
                auto gmcMocs = gmc->getMocsByClassName(currMoc->getClassName());
                if ( gmcMocs.size() > 0 )
                {
                    GMCManagedObject * currGmcMoc = gmcMocs[0];
                    PDDBManagedObjectParameter * currComplexPar = (PDDBManagedObjectParameter*)r.getSecondElementComplexTypeParameter();

                    // There are mocs like these
                    auto gmcComplexPar = currGmcMoc->getParameterByName(currPar->getName());
                    if ( gmcComplexPar == NULL )
                    {
                        if ( currPar->getCreationPriority() == "mandatory" && currPar->isVendorSpecific() == false )
                        {
                            bool refsFoundPost = isFoundInGMCAction( currComplexPar->getRelatedParameters(), actionsPost);
                            bool refsFoundStandard = isAnyOfReferencesValid(currComplexPar->getRelatedParameters(), gmc);
                            if ( refsFoundStandard )
                            {
                                actions.push_back( GMCAction(r, true, currPar, GMCAction::ActionType::Add,
                                                              GMCAction::ChangeScope::ComplexParameter,
                                                              r.getId(),"Related Parameters found in GMC", gmcMocs) );
                            }
                            else
                            {
                                if ( refsFoundPost )
                                {
                                    actions.push_back( GMCAction(r, true, currPar, GMCAction::ActionType::Add,
                                                                  GMCAction::ChangeScope::ComplexParameter,
                                                                  r.getId(),"Related Parameters found in GMCActions", gmcMocs) );
                                }
                            }
                        }
                        else
                        {
                            if ( currPar->getCreationPriority() == "optional" && currPar->isVendorSpecific() == false )
                            {

                                bool refsFoundPost = isFoundInGMCAction( currComplexPar->getRelatedParameters(), actionsPost);
                                bool refsFoundStandard = isAnyOfReferencesValid(currComplexPar->getRelatedParameters(), gmc);
                                if ( refsFoundStandard )
                                {
                                    actions.push_back( GMCAction(r, true, currPar, GMCAction::ActionType::Add,
                                                                  GMCAction::ChangeScope::ComplexParameter,
                                                                  r.getId(),"Related Parameters found in GMC", gmcMocs) );
                                }
                                else
                                {
                                    if ( refsFoundPost )
                                    {
                                        actions.push_back( GMCAction(r, true, currPar, GMCAction::ActionType::Add,
                                                                      GMCAction::ChangeScope::ComplexParameter,
                                                                      r.getId(),"Related Parameters found in GMCActions", gmcMocs) );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        //if ( r. )
    }

    return actions;
}

std::vector<GMCAction> GMCDocument::resolveGMCActions( PDDBDocument * oldPDDB, PDDBDocument * newPDDB, GMCDocument * gmc, std::vector<PDDBManagedObjectCompareResult> * compareResults )
{
    vector<GMCAction> preActionList = resolveGMCActionsPostProcessing(oldPDDB, newPDDB, gmc, compareResults, vector<GMCAction>());
    vector<GMCAction> postActionList = resolveGMCActionsPostProcessing(oldPDDB, newPDDB, gmc, compareResults, preActionList);
    return postActionList;
}


std::pair< std::string, std::string> GMCDocument::resolveGMCCompareText(GMCDocument * original, GMCDocument * modified, GMCAction action)
{
    pair<string, string> result;
    result.first = "";
    result.second = "";
    try
    {
        if ( action.getActionType() == GMCAction::Add )
        {
            if ( action.getChangeScope() == GMCAction::ManagedObject )
            {
                auto mocInPDDB = ((PDDBManagedObject*)action.getItem());
                auto className = mocInPDDB->getClassName();
                auto mocFound = modified->getManagedObjectByClassName(className);
                if ( mocFound == NULL )
                    return result;
                result.second = XmlElementReader::getXML( mocFound->getElement() );
            }
            if ( action.getChangeScope() == GMCAction::ManagedObjectParameter )
            {
                auto par = ((PDDBManagedObjectParameter*)action.getItem());
                auto parParent = (PDDBManagedObject*)par->getMocParent();
                auto gmcMocFound = modified->getManagedObjectByClassName(parParent->getClassName());
                if ( gmcMocFound == NULL )
                    return result;
                string parName = par->getName();
                auto foundParInGMC = gmcMocFound->getParameterByName(parName);
                if ( foundParInGMC == NULL )
                    return result;
                result.second = XmlElementReader::getXML(foundParInGMC->getElement());
            }

            if ( action.getChangeScope() == GMCAction::ComplexParameter )
            {
                auto par = ((PDDBManagedObjectParameter*)action.getItem());
                auto parParent = (PDDBManagedObject*)par->getMocParent();
                auto gmcMocFound = modified->getManagedObjectByClassName(parParent->getClassName());
                if ( gmcMocFound == NULL )
                    return result;
                string parName = par->getName();
                auto foundParInGMC = gmcMocFound->getParameterByName(parName);
                if ( foundParInGMC == NULL )
                    return result;
                result.second = XmlElementReader::getXML(foundParInGMC->getElement());
            }
        }

        if ( action.getActionType() == GMCAction::Remove )
        {
            result.second = "";
            if ( action.getChangeScope() == GMCAction::ManagedObject )
                result.first = XmlElementReader::getXML( original->getManagedObjectByClassName(((PDDBManagedObject*)action.getItem())->getClassName())->getElement() );
            if ( action.getChangeScope() == GMCAction::ManagedObjectParameter )
                result.first = XmlElementReader::getXML( original->getManagedObjectByClassName(
                                              ((PDDBManagedObject*)((PDDBManagedObjectParameter*)action.getItem())->getMocParent())->getClassName())
                                          ->getParameterByName(((PDDBManagedObjectParameter*)action.getItem())->getName() )->getElement() );
        }

        if ( action.getActionType() == GMCAction::Modify )
        {
            if ( action.getChangeScope() == GMCAction::ManagedObject )
                result.first = XmlElementReader::getXML( original->getManagedObjectByClassName(((PDDBManagedObject*)action.getItem())->getClassName())->getElement() );
            if ( action.getChangeScope() == GMCAction::ManagedObjectParameter )
            {
                auto parm = (PDDBManagedObjectParameter*)action.getItem();
                auto parmParent =(PDDBManagedObject*)(parm->getMocParent());
                auto mocgmcorig = original->getManagedObjectByClassName(parmParent->getClassName());
                if ( mocgmcorig == NULL )
                    return result;
                auto mocorigparam = mocgmcorig->getParameterByName(parm->getName());
                if ( mocorigparam == NULL )
                    return result;
                result.first = XmlElementReader::getXML(mocorigparam->getElement() );
            }

            if ( action.getChangeScope() == GMCAction::ManagedObjectParameter )
            {
                auto par = ((PDDBManagedObjectParameter*)action.getItem());
                auto parParent = (PDDBManagedObject*)par->getMocParent();
                auto gmcMocFound = modified->getManagedObjectByClassName(parParent->getClassName());
                if ( gmcMocFound == NULL )
                    return result;
                string parName = par->getName();
                auto foundParInGMC = gmcMocFound->getParameterByName(parName);
                if ( foundParInGMC == NULL )
                    return result;
                result.second = XmlElementReader::getXML(foundParInGMC->getElement());
            }
        }
    }
    catch(...)
    {
        return result;
    }

    return result;
}
