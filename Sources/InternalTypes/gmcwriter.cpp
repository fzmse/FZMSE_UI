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

XMLElement * GMCWriter::insertMoc(GMCDocument * gmc, std::string className)
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
        XmlWriter::insertChild( (XMLElement*)gmc->getXMLDocument(), n);
        return n;
    }
    else
    {
        // insert after
        XMLElement * n = gmc->getXMLDocument()->NewElement("managedObject");
        n->SetAttribute("class", className.c_str());
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

void GMCWriter::reactToAction(GMCDocument * gmc, GMCAction action)
{
    if ( action.getActionType() == GMCAction::ActionType::Add )
    {
        if ( action.getChangeScope() == GMCAction::ChangeScope::ManagedObject )
        {
            insertMoc(gmc, ((PDDBManagedObject*)action.getItem())->getClassName());
            gmc->reinitialize();
            GMCManagedObject * gmcNewMoc = gmc->getManagedObjectByClassName( ((PDDBManagedObject*)action.getItem())->getClassName());
            if ( action.getChildActions().size() > 0 )
            {
                vector<GMCAction> childActions = action.getChildActions();
                for ( vector<GMCAction>::iterator it = childActions.begin(); it != childActions.end(); it ++ )
                {
                    GMCAction a = *it;
                    PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                    PDDBDefaultValue * val = p->getPDDBValue();
                    if ( val->isComplexType() == false )
                    {
                        insertParameterSimpleType(gmc, gmcNewMoc, p->getName(), ((PDDBSimpleTypeValue*)val)->getEvaluatedValue() );
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
                            parPair.second = ((PDDBSimpleTypeValue*)(*pIt)->getPDDBValue())->getEvaluatedValue();
                            nameVal.push_back(parPair);
                        }

                        insertParameterComplexType(gmc, gmcNewMoc, p->getName(), nameVal);
                        gmc->reinitialize();

                    }
                }
            }
            delete gmcNewMoc;
        }
        else
        {
            if ( action.getChangeScope() == GMCAction::ChangeScope::ManagedObjectParameter )
            {
                for ( vector<GMCManagedObject*>::iterator it = action.getGmcMocsInvolved().begin();
                      it != action.getGmcMocsInvolved().end(); it ++ )
                {
                    GMCAction a = action;
                    PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                    PDDBDefaultValue * val = p->getPDDBValue();
                    if ( val->isComplexType() == false )
                    {
                        insertParameterSimpleType(gmc, (*it), p->getName(), ((PDDBSimpleTypeValue*)val)->getEvaluatedValue() );
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
                            parPair.second = ((PDDBSimpleTypeValue*)(*pIt)->getPDDBValue())->getEvaluatedValue();
                            nameVal.push_back(parPair);
                        }

                        insertParameterComplexType(gmc, (*it), p->getName(), nameVal);
                        gmc->reinitialize();

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
            auto els = action.getGmcMocsInvolved();
            for ( vector<GMCManagedObject*>::iterator it = els.begin();
                  it != els.end(); it ++ )
            {
                removeMoc(gmc, *it );
                gmc->reinitialize();
            }
        }

        if ( action.getChangeScope() == GMCAction::ManagedObjectParameter )
        {
            auto els = action.getGmcMocsInvolved();
            for ( vector<GMCManagedObject*>::iterator it = els.begin();
                  it != els.end(); it ++ )
            {

                GMCAction a = action;
                PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                PDDBDefaultValue * val = p->getPDDBValue();
                if ( val->isComplexType() == false )
                {
                    removeParameterSimpleType(gmc, (*it), p->getName());
                    gmc->reinitialize();
                }
                else
                {
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
            auto els = action.getGmcMocsInvolved();
            for ( vector<GMCManagedObject*>::iterator it = els.begin();
                  it != els.end(); it ++ )
            {
                GMCAction a = action;
                PDDBManagedObjectParameter * p = (PDDBManagedObjectParameter*)a.getItem();
                PDDBDefaultValue * val = p->getPDDBValue();

                GMCManagedObjectParameter * gmcParam = (*it)->getParameterByName(p->getName());
                if ( val->isComplexType() == false )
                {
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

    gmc->reinitialize();
}


void GMCWriter::reactToAllWithoutReaderInteraction( GMCDocument * gmc, std::vector<GMCAction> actions)
{
    for ( vector<GMCAction>::iterator it = actions.begin(); it != actions.end(); it ++ )
    {
        if ((*it).isReaderInteractionRequired() == false)
            reactToAction(gmc, *it);
    }
}
