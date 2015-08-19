
#include "InternalTypes/GMCManagedObject.h"

using namespace InternalTypes;
using namespace std;
using namespace tinyxml2;

GMCManagedObject::GMCManagedObject(XMLElement *e) :
    ManagedObjectRelativeElement(e)
{
    this->validMocObject = false;
    if ( e != NULL )
    {
        if ( XmlElementReader::getName(e) == MANAGED_OBJECT_XML_NAME )
        {
            this->validMocObject = true;

            this->className = retrieveClassName(e);

            vector<XMLElement * > pElements = XmlReader::getChildren(e, "");
            for ( vector<XMLElement * >::iterator it = pElements.begin(); it != pElements.end(); ++ it )
            {
                this->parameters.push_back( new GMCManagedObjectParameter(*it, this) );
            }
        }

    }
}

GMCManagedObject::~GMCManagedObject()
{

}

GMCManagedObjectParameter * GMCManagedObject::getParameterByName(std::string name)
{
    for ( std::vector<GMCManagedObjectParameter*>::iterator it = this->parameters.begin(); it != this->parameters.end(); it ++)
    {
        if ( (*it)->getName() == name )
            return (*it);
    }
    return NULL;
}

std::vector<GMCManagedObjectParameter* > GMCManagedObject::getParameters()
{
    return this->parameters;
}

std::string GMCManagedObject::getClassName()
{
    return this->className;
}

std::string GMCManagedObject::retrieveClassName(XMLElement * e)
{
    return XmlElementReader::getAttributeByName(e, "class");
}
