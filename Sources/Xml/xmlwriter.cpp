
#include "Xml/XmlWriter.h"

using namespace tinyxml2;
using namespace std;

tinyxml2::XMLDocument * XmlWriter::createDocumentFrom(tinyxml2::XMLDocument * doc)
{
    tinyxml2::XMLDocument * newDoc = new tinyxml2::XMLDocument();
    std::string docXml = XmlElementReader::getXML((tinyxml2::XMLElement*)doc);
    newDoc->Parse(docXml.c_str());
    return newDoc;
}

tinyxml2::XMLElement * XmlWriter::insertAfter(tinyxml2::XMLElement * element, tinyxml2::XMLNode * n)
{
    XMLElement * parent = (XMLElement *)element->Parent();
    if ( parent == NULL )
        return NULL;
    else
        return (XMLElement*)parent->InsertAfterChild(element, n);
}

tinyxml2::XMLElement * XmlWriter::insertChild(tinyxml2::XMLElement * element, tinyxml2::XMLNode * n)
{
    return (XMLElement*)element->InsertFirstChild(n);
}

bool XmlWriter::save(tinyxml2::XMLDocument * doc, std::string filename, bool sort, bool sortParams)
{
    if ( sort )
        sortMocs(doc);
    if ( sortParams )
        sortParameters(doc);
    tinyxml2::XMLError err = doc->SaveFile(filename.c_str());
    if ( err == tinyxml2::XMLError::XML_SUCCESS || err == tinyxml2::XMLError::XML_NO_ERROR )
        return true;
    else
        return false;
}

inline bool isAlreadyInFound(vector<XMLElement*> * found, XMLElement * e)
{
    bool alredyInFound = false;
    for ( int u = 0; u < found->size(); u ++ )
    {
        if ( ((*found)[u]) == e )
        {
            alredyInFound = true;
            break;
        }
    }
    return alredyInFound;
}

inline string resolveMocSortText(XMLElement * e)
{
    std::string value = XmlElementReader::getAttributeByName(e, "class");
    value += XmlElementReader::getAttributeByName(e, "distName"); // "" if no distName ( PDDB )
    return value;
}

inline string resolveMocParSortText(XMLElement * e)
{
    return XmlElementReader::getAttributeByName(e, "name");
}

inline XMLElement * pickSmallestElementFromNotYetFound(vector<XMLElement*> all, vector<XMLElement*> * found, std::string (*resolveSortTextFunc)(XMLElement*)  )
{
    XMLElement * smallestElement = NULL;
    for ( int i = 0; i < all.size(); i ++ )
    {

        XMLElement * e = all[i];
        string currentValue = resolveSortTextFunc(e);

        if ( smallestElement == NULL )
        {
            if ( isAlreadyInFound(found, e) == false )
                smallestElement = e;
        }
        else
        {
            string smallestValueSoFar = resolveSortTextFunc(smallestElement);
            if ( currentValue <= smallestValueSoFar )
            {
                if ( isAlreadyInFound(found, e) == false )
                    smallestElement = e;
            }
        }
    }
    return smallestElement;

}

XMLNode *deepCopy( XMLNode *src, XMLDocument *destDoc )
{
    XMLNode *current = src->ShallowClone( destDoc );
    for( XMLNode *child=src->FirstChild(); child; child=child->NextSibling() )
    {
        current->InsertEndChild( deepCopy( child, destDoc ) );
    }

    return current;
}

void XmlWriter::sortMocs(tinyxml2::XMLDocument * doc)
{
    XMLElement * firstMoc = XmlReader::getFirstElementWithSpecificNameAndAttribute((XMLElement*)doc, "managedObject");
    if ( firstMoc == NULL )
        return;

    XMLNode * parentNode = firstMoc->Parent();
    if ( parentNode == NULL )
        return;

    vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel((XMLElement*)parentNode, "managedObject");
    vector<XMLElement *> found;

    while( XMLElement * smallestElement = pickSmallestElementFromNotYetFound(elems, &found, resolveMocSortText))
    {
        found.push_back(smallestElement);
    }

    bool allSorted = elems.size() == found.size();

    if ( allSorted )
    {
        std::vector<XMLNode *> newNodes;
        for ( vector<XMLElement *>::iterator it = found.begin(); it != found.end(); it ++ )
        {
            XMLElement * clone = (XMLElement*)(deepCopy(*it, doc));
            newNodes.push_back(clone);
        }

        XMLElement * parentElement = (XMLElement*)parentNode;

        for ( int i = 0; i < elems.size(); i ++ )
            parentElement->DeleteChild(elems[i]);

        for ( int i = 0; i < newNodes.size(); i ++ )
            parentElement->InsertEndChild(newNodes[i]);

    }

}


void XmlWriter::sortParameters(tinyxml2::XMLDocument * doc)
{
    XMLElement * firstMoc = XmlReader::getFirstElementWithSpecificNameAndAttribute((XMLElement*)doc, "managedObject");
    if ( firstMoc == NULL )
        return;

    XMLNode * parentNode = firstMoc->Parent();
    if ( parentNode == NULL )
        return;

    vector<XMLElement *> elems = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel((XMLElement*)parentNode, "managedObject");
    for ( vector<XMLElement *>::iterator it = elems.begin(); it != elems.end(); it ++ )
    {
        vector<XMLElement *> listChildren = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(*it, "list");
        vector<XMLElement *> parChildren = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(*it, "p");

        vector<XMLElement *> foundList;
        vector<XMLElement *> foundPar;

        // Sort insides of lists
        for ( vector<XMLElement *>::iterator itListPars = listChildren.begin(); itListPars != listChildren.end(); itListPars ++ )
        {
            vector<XMLElement *> foundListPar;
            vector<XMLElement *> listParChildren = XmlReader::getElementsWithSpecificNameAndAttribute(*itListPars, "item");

            // item
            for (  vector<XMLElement *>::iterator itListParsItem = listParChildren.begin(); itListParsItem != listParChildren.end(); itListParsItem ++  )
            {
                foundListPar.clear();
                vector<XMLElement*> listParPar = XmlReader::getElementsWithSpecificNameAndAttributeFromChildrenLevel(*itListParsItem, "p");

                while( XMLElement * smallestElement = pickSmallestElementFromNotYetFound(listParPar, &foundListPar, resolveMocParSortText))
                    foundListPar.push_back(smallestElement);

                bool allSorted = listParPar.size() == foundListPar.size();

                XMLElement * parentListPar = *itListParsItem;
                if ( allSorted && parentListPar != NULL)
                {
                    std::vector<XMLNode *> newNodesListPar;
                    for ( vector<XMLElement *>::iterator itx = foundListPar.begin(); itx != foundListPar.end(); itx ++ )
                    {
                        XMLElement * clone = (XMLElement*)(deepCopy(*itx, doc));
                        newNodesListPar.push_back(clone);
                    }

                    // List Par
                    for ( int i = 0; i < listParPar.size(); i ++ )
                        parentListPar->DeleteChild(listParPar[i]);

                    for ( int i = 0; i < newNodesListPar.size(); i ++ )
                        parentListPar->InsertEndChild(newNodesListPar[i]);
                }
            }
        }

        while( XMLElement * smallestElement = pickSmallestElementFromNotYetFound(listChildren, &foundList, resolveMocParSortText))
            foundList.push_back(smallestElement);

        while( XMLElement * smallestElement = pickSmallestElementFromNotYetFound(parChildren, &foundPar, resolveMocParSortText))
            foundPar.push_back(smallestElement);

        bool allSorted = listChildren.size() == foundList.size() && parChildren.size() == foundPar.size();

        if ( allSorted )
        {
            std::vector<XMLNode *> newNodesList;
            for ( vector<XMLElement *>::iterator itx = foundList.begin(); itx != foundList.end(); itx ++ )
            {
                XMLElement * clone = (XMLElement*)(deepCopy(*itx, doc));
                newNodesList.push_back(clone);
            }

            std::vector<XMLNode *> newNodesPar;
            for ( vector<XMLElement *>::iterator itx = foundPar.begin(); itx != foundPar.end(); itx ++ )
            {
                XMLElement * clone = (XMLElement*)(deepCopy(*itx, doc));
                newNodesPar.push_back(clone);
            }


            XMLElement * parentElement = *it;

            // List
            for ( int i = 0; i < listChildren.size(); i ++ )
                parentElement->DeleteChild(listChildren[i]);

            for ( int i = 0; i < newNodesList.size(); i ++ )
                parentElement->InsertEndChild(newNodesList[i]);


            // List items


            // Par
            for ( int i = 0; i < parChildren.size(); i ++ )
                parentElement->DeleteChild(parChildren[i]);

            for ( int i = 0; i < newNodesPar.size(); i ++ )
                parentElement->InsertEndChild(newNodesPar[i]);

        }
    }
}
