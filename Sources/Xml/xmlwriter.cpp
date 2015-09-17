
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

bool XmlWriter::save(tinyxml2::XMLDocument * doc, std::string filename, bool sort)
{
    if ( sortMocs )
        sortMocs(doc);
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

inline XMLElement * pickSmallestElementFromNotYetFound(vector<XMLElement*> all, vector<XMLElement*> * found)
{
    XMLElement * smallestElement = NULL;
    for ( int i = 0; i < all.size(); i ++ )
    {

        XMLElement * e = all[i];
        string currentValue = resolveMocSortText(e);

        if ( smallestElement == NULL )
        {
            if ( isAlreadyInFound(found, e) == false )
                smallestElement = e;
        }
        else
        {
            string smallestValueSoFar = resolveMocSortText(smallestElement);
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

    while( XMLElement * smallestElement = pickSmallestElementFromNotYetFound(elems, &found))
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
