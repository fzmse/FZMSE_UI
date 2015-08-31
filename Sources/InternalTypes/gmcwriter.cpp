#include "InternalTypes/GMCWriter.h"

using namespace InternalTypes;
using namespace std;
using namespace tinyxml2;

void GMCWriter::insertParameterSimpleType(GMCDocument * gmc, GMCManagedObject * moc,
                                      std::string name, std::string value )
{
    vector<GMCManagedObject*> mocsToInsert = gmc->getMocsByClassName(moc->getClassName());
    for ( vector<GMCManagedObject*>::iterator it = mocsToInsert.begin(); it != mocsToInsert.end(); it ++ )
    {
        // Insert to moc
        GMCManagedObject * m = *it;
        vector<GMCManagedObjectParameter*> pars = m->getParameters();

        XMLElement * insertAfter = NULL;
        vector<XMLElement*> elements;

        elements = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(m->getElement(), "");

        for ( vector<XMLElement*>::iterator pIt = elements.begin(); pIt != elements.end(); pIt ++ )
        {
            XMLElement * currEl = *pIt;
            string currParName = XmlElementReader::getAttributeByName(currEl, "name");

            if ( XmlElementReader::getName(currEl) != "p" )
                insertAfter = currEl;
            else
            {
                if ( name < currParName )
                    break;
                insertAfter = currEl;
            }
        }

        if ( insertAfter == NULL )
        {
            // insert first child
            XMLElement * n = gmc->getXMLDocument()->NewElement("p");
            n->SetAttribute("name", name.c_str());
            n->SetText(value.c_str());
            XmlWriter::insertChild( m->getElement(), n);
        }
        else
        {
            // insert after
            XMLElement * n = gmc->getXMLDocument()->NewElement("p");
            n->SetAttribute("name", name.c_str());
            n->SetText(value.c_str());
            XmlWriter::insertAfter( insertAfter, n);
        }

    }
}

void GMCWriter::insertParameterComplexType(GMCDocument * gmc, GMCManagedObject * moc, std::string name,
                                       std::vector< std::pair< std::string, std::string> > values)
{
    vector<GMCManagedObject*> mocsToInsert = gmc->getMocsByClassName(moc->getClassName());
    for ( vector<GMCManagedObject*>::iterator it = mocsToInsert.begin(); it != mocsToInsert.end(); it ++ )
    {
        // Insert to moc
        GMCManagedObject * m = *it;
        vector<GMCManagedObjectParameter*> pars = m->getParameters();

        XMLElement * insertAfter = NULL;
        vector<XMLElement*> elements;

        elements = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(m->getElement(), "");

        for ( vector<XMLElement*>::iterator pIt = elements.begin(); pIt != elements.end(); pIt ++ )
        {
            XMLElement * currEl = *pIt;
            string currParName = XmlElementReader::getAttributeByName(currEl, "name");

            if ( XmlElementReader::getName(currEl) < "list" )
                insertAfter = currEl;
            else
            {
                if ( XmlElementReader::getName(currEl) > "list" )
                    break;
                else
                {
                    if ( name < currParName )
                        break;
                    insertAfter = currEl;
                }
            }
        }

        // insert first child
        XMLElement * n = gmc->getXMLDocument()->NewElement("list");
        XMLElement * itemEl = gmc->getXMLDocument()->NewElement("item");
        n->SetAttribute("name", name.c_str());
        n->InsertFirstChild(itemEl);
        for ( std::vector< std::pair< std::string, std::string> > ::iterator pInsertIt = values.begin();
              pInsertIt != values.end(); pInsertIt ++ )
        {
            XMLElement * p = gmc->getXMLDocument()->NewElement("p");
            p->SetAttribute("name", (*pInsertIt).first.c_str());
            p->SetText((*pInsertIt).second.c_str());
            itemEl->InsertEndChild(p);
        }

        if ( insertAfter == NULL )
        {
            XmlWriter::insertChild( m->getElement(), n);
        }
        else
        {
            // insert after
            XmlWriter::insertAfter( insertAfter, n);
        }
    }

}

XMLElement * GMCWriter::insertMoc(GMCDocument * gmc, std::string className, std::string version)
{
    XMLElement * insertAfter = NULL;
    GMCManagedObject * firstMoc = gmc->getManagedObjects()[0];
    if ( firstMoc != NULL )
    {
        vector<XMLElement*> elements = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel((XMLElement*)firstMoc->getElement()->Parent(), "");
        for ( vector<XMLElement*>::iterator it = elements.begin(); it != elements.end(); it ++ )
        {
            if ( XmlElementReader::getName(*it) < "managedObject" )
                insertAfter = *it;
            else
            {
                if ( className < XmlElementReader::getAttributeByName(*it, "class") )
                    break;
                else
                    insertAfter = *it;
            }
        }


    }


    if ( insertAfter == NULL )
    {
        // insert first child
        XMLElement * n = gmc->getXMLDocument()->NewElement("managedObject");
        n->SetAttribute("class", className.c_str());
        n->SetAttribute("operation", "create");
        n->SetAttribute("version", version.c_str());

        XmlWriter::insertChild( (XMLElement*)gmc->getXMLDocument(), n);
        return n;
    }
    else
    {
        // insert after
        XMLElement * n = gmc->getXMLDocument()->NewElement("managedObject");
        n->SetAttribute("class", className.c_str());
        n->SetAttribute("operation", "create");
        n->SetAttribute("version", version.c_str());
        XmlWriter::insertAfter( insertAfter, n);
        return n;
    }

}

void GMCWriter::removeParameterSimpleType(GMCDocument * gmc, GMCManagedObject * moc, std::string name)
{

    vector<GMCManagedObject*> mocsToDelete = gmc->getMocsByClassName(moc->getClassName());
    for ( vector<GMCManagedObject*>::iterator it = mocsToDelete.begin(); it != mocsToDelete.end(); it ++ )
    {
        // Delete moc par
        GMCManagedObject * m = *it;
        vector<XMLElement*> elements;
        XMLElement* elementToRemove = NULL;
        elements = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(m->getElement(), "p");

        for ( vector<XMLElement*>::iterator pIt = elements.begin(); pIt != elements.end(); pIt ++ )
        {
            XMLElement * currEl = *pIt;
            string currParName = XmlElementReader::getAttributeByName(currEl, "name");

            if ( currParName == name )
                elementToRemove = currEl;
        }

        if ( elementToRemove != NULL )
            m->getElement()->DeleteChild(elementToRemove);

    }
}
void GMCWriter::removeParameterComplexType(GMCDocument * gmc, GMCManagedObjectParameter * complexPar,
                                       std::string name)
{
    GMCManagedObject * parMoc = (GMCManagedObject*)complexPar->getParentMoc();

    GMCManagedObject * m = gmc->getManagedObjectByClassName(parMoc->getClassName());
    GMCManagedObjectParameter * mPar = m->getParameterByName(complexPar->getName());
    vector<XMLElement * > matches = XmlReader::getElementsWithSpecificNameAndAttribute(mPar->getElement(), "p", "name", name);
    for ( vector<XMLElement * >::iterator it = matches.begin(); it != matches.end(); it ++ )
    {
        XMLElement * currEl = (*it);
        currEl->Parent()->DeleteChild(currEl);
    }
}
void GMCWriter::removeMoc(GMCDocument * gmc, GMCManagedObject * moc)
{
    GMCManagedObject * m = gmc->getManagedObjectByClassName(moc->getClassName());

    if ( m != NULL )
    {
        gmc->getXMLDocument()->DeleteChild(m->getElement());
    }

}

void GMCWriter::modifyParameterSimpleType(GMCDocument * gmc, GMCManagedObjectParameter * par,
                                      std::string value)
{
    GMCManagedObject * parMoc = (GMCManagedObject*)par->getParentMoc();

    GMCManagedObject * m = gmc->getManagedObjectByClassName(parMoc->getClassName());
    GMCManagedObjectParameter * p = m->getParameterByName(par->getName());
    if ( p != NULL )
    {
        p->getElement()->SetText(value.c_str()); // TODO
    }
}


void GMCWriter::modifyParameterComplexType(GMCDocument * gmc, GMCManagedObjectParameter * complexPar,
                                       std::string name, std::string value)
{
    GMCManagedObject * parMoc = (GMCManagedObject*)complexPar->getParentMoc();

    GMCManagedObject * m = gmc->getManagedObjectByClassName(parMoc->getClassName());
    GMCManagedObjectParameter * mPar = m->getParameterByName(complexPar->getName());
    vector<XMLElement * > matches = XmlReader::getElementsWithSpecificNameAndAttribute(mPar->getElement(), "p", "name", name);
    for ( vector<XMLElement * >::iterator it = matches.begin(); it != matches.end(); it ++ )
    {
        (*it)->SetText(value.c_str()); // TODO
    }
}


inline std::string getAddReason(PDDBManagedObjectCompareResult r, PDDBManagedObjectParameter * p)
{
    std::string result = "";
    if ( r.containsChange(PDDBManagedObjectCompareResult::CreationPriority) )
        result += "changed to mandatory in PDDB";
    else
        result += "added in PDDB";
    if ( p->getLteName() != "" )
    {
        result += " ( ";
        result += p->getLteName();
        result += " )";
    }
    return result;
}

inline std::string getAddReason(PDDBManagedObjectCompareResult r, PDDBManagedObject * moc)
{
    std::string result = "";
    if ( r.containsChange(PDDBManagedObjectCompareResult::CreationPriority) )
        result += "changed to mandatory in PDDB";
    else
        result += "added in PDDB";

    return result;
}

inline std::string getRemoveReason(PDDBManagedObjectCompareResult r, PDDBManagedObjectParameter * p)
{
    std::string result = "";
    if ( r.containsChange(PDDBManagedObjectCompareResult::CreationPriority) && p->getCreationPriority() == "optional" )
        result += "changed to optional in PDDB";
    else
        result += "deleted in PDDB";
    return result;
}

inline std::string getModifyReason(PDDBManagedObjectCompareResult r, PDDBManagedObjectParameter * p)
{
    std::string result = "";

    if ( p->getLteName() != "" )
    {
        result += "( ";
        result += p->getLteName();
        result += " )";
    }
    return result;
}

vector<ReportEntry> GMCWriter::reactToAction(GMCDocument * gmc, GMCAction action)
{
    vector<ReportEntry> entries;
    if ( action.getActionType() == GMCAction::ActionType::Add )
    {
        if ( action.getChangeScope() == GMCAction::ChangeScope::ManagedObject )
        {
            string mocClassName = ((PDDBManagedObject*)action.getItem())->getClassName();

            entries.push_back(ReportEntry(ReportEntry::Add, mocClassName, getAddReason(action.getCompareResult(), ((PDDBManagedObject*)action.getItem())), "", "class added"));

            insertMoc(gmc, ((PDDBManagedObject*)action.getItem())->getClassName(), ((PDDBManagedObject*)action.getItem())->getVersion());
            gmc->reinitialize();
            GMCManagedObject * gmcNewMoc = gmc->getManagedObjectByClassName( mocClassName );
            if ( action.getChildActions().size() > 0 )
            {
                bool justOnceIncludedInReport = false;
                vector<GMCAction> childActions = action.getChildActions();
                for ( vector<GMCAction>::iterator it = childActions.begin(); it != childActions.end(); it ++ )
                {
                    GMCAction a = *it;
                    PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                    PDDBDefaultValue * val = p->getPDDBValue();
                    if ( val->isComplexType() == false )
                    {
                        if ( justOnceIncludedInReport == false )
                        {
                            string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                            paramString += " / ";
                            paramString += p->getName();
                            entries.push_back(ReportEntry(ReportEntry::Add, paramString, getAddReason(action.getCompareResult(), p), "", ((PDDBSimpleTypeValue*)val)->getEvaluatedValueForAdd()));
                            justOnceIncludedInReport = true;
                        }

                        insertParameterSimpleType(gmc, gmcNewMoc, p->getName(), ((PDDBSimpleTypeValue*)val)->getEvaluatedValueForAdd() );
                        gmc->reinitialize();
                    }
                    else
                    {
                        PDDBComplexTypeValue* complexVal = (PDDBComplexTypeValue*)val;
                        std::vector<pair< string, string> > nameVal;
                        auto els = complexVal->getValueParameters();
                        for ( vector<PDDBManagedObjectParameter*>::iterator pIt = els.begin();
                              pIt != els.end(); pIt ++)
                        {
                            pair<string, string> parPair;
                            parPair.first = (*pIt)->getName();
                            parPair.second = ((PDDBSimpleTypeValue*)(*pIt)->getPDDBValue())->getEvaluatedValueForAdd();
                            nameVal.push_back(parPair);
                        }

                        if ( justOnceIncludedInReport == false )
                        {
                            string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                            paramString += " / ";
                            paramString += p->getName();
                            entries.push_back(ReportEntry(ReportEntry::Add, paramString, getAddReason(action.getCompareResult(), p), "", "structure added"));
                            justOnceIncludedInReport = true;
                        }

                        insertParameterComplexType(gmc, gmcNewMoc, p->getName(), nameVal);
                        gmc->reinitialize();

                    }
                }
            }
            //delete gmcNewMoc;
        }
        else
        {
            if ( action.getChangeScope() == GMCAction::ChangeScope::ManagedObjectParameter )
            {
                auto invMocs = action.getGmcMocsInvolved();
                bool justOnceIncludedInReport = false;
                for ( vector<GMCManagedObject*>::iterator it = invMocs.begin();
                      it != invMocs.end(); it ++ )
                {
                    GMCAction a = action;
                    PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                    PDDBDefaultValue * val = p->getPDDBValue();
                    if ( val->isComplexType() == false )
                    {
                        if ( justOnceIncludedInReport == false )
                        {
                            string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                            paramString += " / ";
                            paramString += p->getName();

                            entries.push_back(ReportEntry(ReportEntry::Add, paramString, getAddReason(action.getCompareResult(), p), "", ((PDDBSimpleTypeValue*)val)->getEvaluatedValueForAdd()));
                            justOnceIncludedInReport = true;
                        }

                        insertParameterSimpleType(gmc, (*it), p->getName(), ((PDDBSimpleTypeValue*)val)->getEvaluatedValueForAdd() );
                        gmc->reinitialize();
                    }
                    else
                    {
                        PDDBComplexTypeValue* complexVal = (PDDBComplexTypeValue*)val;
                        std::vector<pair< string, string> > nameVal;
                        auto els = complexVal->getValueParameters();
                        for ( vector<PDDBManagedObjectParameter*>::iterator pIt = els.begin();
                              pIt != els.end(); pIt ++)
                        {
                            pair<string, string> parPair;
                            parPair.first = (*pIt)->getName();
                            parPair.second = ((PDDBSimpleTypeValue*)(*pIt)->getPDDBValue())->getEvaluatedValueForAdd();
                            nameVal.push_back(parPair);
                        }
                        if ( justOnceIncludedInReport == false )
                        {
                            string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                            paramString += " / ";
                            paramString += p->getName();
                            entries.push_back(ReportEntry(ReportEntry::Add, paramString, getAddReason(action.getCompareResult(), p), "", "structure added"));
                            justOnceIncludedInReport = true;
                        }

                        insertParameterComplexType(gmc, (*it), p->getName(), nameVal);
                        gmc->reinitialize();

                    }
                }
            }
            else
            {
                if ( action.getChangeScope() == GMCAction::ChangeScope::ComplexParameter )
                {
                    bool justOnceIncludedInReport = false;
                    auto mocsInv = action.getGmcMocsInvolved();
                    for ( vector<GMCManagedObject*>::iterator it = mocsInv.begin();
                          it != mocsInv.end(); it ++ )
                    {
                        GMCAction a = action;
                        PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                        PDDBDefaultValue * val = p->getPDDBValue();
                        if ( val->isComplexType() == false )
                        {
                            if ( justOnceIncludedInReport == false )
                            {
                                string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                                paramString += " / ";
                                paramString += p->getName();
                                entries.push_back(ReportEntry(ReportEntry::Add, paramString, getAddReason(action.getCompareResult(), p), "", ((PDDBSimpleTypeValue*)val)->getEvaluatedValueForAdd() ));
                                justOnceIncludedInReport = true;
                            }

                            insertParameterSimpleType(gmc, (*it), p->getName(), ((PDDBSimpleTypeValue*)val)->getEvaluatedValueForAdd() );
                            gmc->reinitialize();
                        }
                        else
                        {
                            PDDBComplexTypeValue* complexVal = (PDDBComplexTypeValue*)val;
                            std::vector<pair< string, string> > nameVal;
                            auto els = complexVal->getValueParameters();
                            for ( vector<PDDBManagedObjectParameter*>::iterator pIt = els.begin();
                                  pIt != els.end(); pIt ++)
                            {
                                pair<string, string> parPair;
                                parPair.first = (*pIt)->getName();
                                parPair.second = ((PDDBSimpleTypeValue*)(*pIt)->getPDDBValue())->getEvaluatedValueForAdd();
                                nameVal.push_back(parPair);
                            }

                            if ( justOnceIncludedInReport == false )
                            {
                                string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                                paramString += " / ";
                                paramString += p->getName();
                                entries.push_back(ReportEntry(ReportEntry::Add, paramString, getAddReason(action.getCompareResult(), p), "", "structure added"));
                                justOnceIncludedInReport = true;
                            }

                            insertParameterComplexType(gmc, (*it), p->getName(), nameVal);
                            gmc->reinitialize();

                        }
                    }
                }
            }
        }
    }

    // Remove

    if ( action.getActionType() == GMCAction::Remove )
    {
        if ( action.getChangeScope() == GMCAction::ManagedObject )
        {
            bool justOnceIncludedInReport = false;
            auto els = action.getGmcMocsInvolved();
            for ( vector<GMCManagedObject*>::iterator it = els.begin();
                  it != els.end(); it ++ )
            {

                if ( justOnceIncludedInReport == false )
                {
                    entries.push_back(ReportEntry(ReportEntry::Remove, (*it)->getClassName(), "deleted in PDDB"));
                    justOnceIncludedInReport = true;
                }

                removeMoc(gmc, *it );
                gmc->reinitialize();
            }
        }

        if ( action.getChangeScope() == GMCAction::ManagedObjectParameter )
        {
            auto els = action.getGmcMocsInvolved();
            bool justOnceIncludedInReport = false;
            for ( vector<GMCManagedObject*>::iterator it = els.begin();
                  it != els.end(); it ++ )
            {

                GMCAction a = action;
                PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                PDDBDefaultValue * val = p->getPDDBValue();
                if ( val->isComplexType() == false )
                {
                    if ( justOnceIncludedInReport == false )
                    {
                        string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                        paramString += " / ";
                        paramString += p->getName();
                        entries.push_back(ReportEntry(ReportEntry::Remove, paramString, getRemoveReason(action.getCompareResult(), p)));
                        justOnceIncludedInReport = true;
                    }
                    removeParameterSimpleType(gmc, (*it), p->getName());
                    gmc->reinitialize();
                }
                else
                {
                    if ( justOnceIncludedInReport == false )
                    {
                        string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                        paramString += " / ";
                        paramString += p->getName();
                        entries.push_back(ReportEntry(ReportEntry::Remove, paramString, getRemoveReason(action.getCompareResult(), p)));
                        justOnceIncludedInReport = true;
                    }
                    GMCManagedObjectParameter * currPar = (*it)->getParameterByName(p->getName());
                    removeParameterComplexType(gmc, currPar, p->getName() );
                    gmc->reinitialize();
                }

            }
        }
    }

    // Modify

    if ( action.getActionType() == GMCAction::Modify)
    {
        if ( action.getChangeScope() == GMCAction::ManagedObjectParameter )
        {
            bool justOnceIncludedInReport = false;
            auto els = action.getGmcMocsInvolved();
            for ( vector<GMCManagedObject*>::iterator it = els.begin();
                  it != els.end(); it ++ )
            {
                GMCAction a = action;
                PDDBManagedObjectParameter * oldP = (PDDBManagedObjectParameter*)a.getOldItem();
                PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                PDDBDefaultValue * val = p->getPDDBValue();

                GMCManagedObjectParameter * gmcParam = (*it)->getParameterByName(p->getName());
                if ( gmcParam != NULL )
                {
                    if ( val->isComplexType() == false )
                    {
                        if ( justOnceIncludedInReport == false )
                        {
                            string paramString = ((PDDBManagedObject*)p->getMocParent())->getClassName();
                            paramString += " / ";
                            paramString += p->getName();
                            entries.push_back(ReportEntry(ReportEntry::Modify, paramString , getModifyReason(action.getCompareResult(), p), ((PDDBSimpleTypeValue*)oldP->getPDDBValue())->getEvaluatedValue() , ((PDDBSimpleTypeValue*)val)->getEvaluatedValue()));
                            justOnceIncludedInReport = true;
                        }
                        modifyParameterSimpleType(gmc, gmcParam, ((PDDBSimpleTypeValue*)val)->getEvaluatedValue() );
                    }
                    else
                    {
                        // NOT HANDLED
                        //removeParameterComplexType(gmc, (*it), p->getName() );
                    }
                }
            }
        }
    }

    gmc->reinitialize();

    return entries;
}


std::vector<ReportEntry> GMCWriter::reactToAll( GMCDocument * gmc, std::vector<GMCAction> actions)
{
    std::vector<ReportEntry> report;
    for ( vector<GMCAction>::iterator it = actions.begin(); it != actions.end(); it ++ )
    {
        auto reports = reactToAction(gmc, *it);
        for ( vector<ReportEntry>::iterator rit = reports.begin(); rit != reports.end(); rit ++ )
            report.push_back(*rit);
    }
    return report;
}

std::vector<ReportEntry> GMCWriter::reactToAllIncluded( GMCDocument * gmc, std::vector<GMCAction> actions)
{
    std::vector<ReportEntry> report;
    for ( vector<GMCAction>::iterator it = actions.begin(); it != actions.end(); it ++ )
    {
        if ((*it).isIncludedInGMC() == true)
        {
            auto reports = reactToAction(gmc, *it);
            for ( vector<ReportEntry>::iterator rit = reports.begin(); rit != reports.end(); rit ++ )
                report.push_back(*rit);
        }
    }
    return report;
}


void GMCWriter::updateVersionInGmc(GMCDocument * gmc, std::string version)
{
    XMLElement * el = XmlReader::getFirstElementWithSpecificNameAndAttribute((XMLElement*)gmc->getXMLDocument(), "log");
    if ( el != NULL )
    {
        if ( el->Parent() != NULL )
        {
            if ( XmlElementReader::getName((XMLElement*)(el->Parent())) == "header" )
            {
                el->SetAttribute("appVersion", version.c_str());

                time_t t = time(0);   // get time now
                struct tm * now = localtime( & t );

                const int BUFFER_SIZE = 50;
                char buff[BUFFER_SIZE];
                for ( int i = 0; i < BUFFER_SIZE; i ++ )
                    buff[i] = 0;

                strftime(buff, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S" , now);

                el->SetAttribute("dateTime", buff);
            }
        }
    }


    auto elements = XmlReader::getElementsWithSpecificNameAndAttribute((XMLElement*)gmc->getXMLDocument(), "managedObject");
    for ( std::vector<XMLElement*>::iterator it = elements.begin(); it != elements.end(); it ++ )
    {
        XMLElement * e = *it;
        e->SetAttribute("version", version.c_str());
    }
}
