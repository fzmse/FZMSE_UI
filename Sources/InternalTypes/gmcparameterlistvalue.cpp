#include "InternalTypes/GMCParameterListValue.h"


using namespace tinyxml2;
using namespace std;
using namespace InternalTypes;

GMCParameterListValue::GMCParameterListValue(XMLElement * e)
    : GMCParameterDefaultValue( e )
{
    resolveValues();
}

GMCParameterListValue::~GMCParameterListValue()
{
}

inline vector<pair< string, string > > getParametersSimple( XMLElement * e )
{
    vector<pair<string, string> > results;
    if ( e != NULL )
    {
        for ( XMLElement * el = e; el > 0; el = el->NextSiblingElement() )
        {
            if ( el != NULL )
            {
                if ( "p" == XmlElementReader::getName(el) )
                {
                    pair<string, string> val;
                    val.first = XmlElementReader::getAttributeByName(el, "name");
                    const char * propValRaw = el->GetText();
                    val.second =  propValRaw == NULL ? "" : propValRaw;
                    results.push_back(val);
                }
            }
        }
    }

    return results;
}

inline vector < vector<pair< string, string > > > getParametersComplex( XMLElement * e )
{
    vector< vector<pair<string, string> > > resultsItems;

    if ( e != NULL )
    {
        for ( XMLElement * el = e; el > 0; el = el->NextSiblingElement() )
        {
            if ( el != NULL )
            {
                XMLElement * firstPar = el->FirstChildElement();
                if ( firstPar != NULL )
                {
                    vector<pair<string, string> > resultsParams;
                    for ( XMLElement * parEl = firstPar; parEl > 0; parEl = parEl->NextSiblingElement() )
                    {
                        if ( "p" == XmlElementReader::getName(firstPar) )
                        {
                            pair<string, string> val;
                            val.first = XmlElementReader::getAttributeByName(firstPar, "name");
                            const char * propValRaw = firstPar->GetText();
                            val.second =  propValRaw == NULL ? "" : propValRaw;
                            resultsParams.push_back(val);
                        }
                    }
                    resultsItems.push_back(resultsParams);
                }
            }
        }
    }

    return resultsItems;
}

void GMCParameterListValue::resolveValues()
{
    vector<pair<string, string> > resultsSimple;
    vector< vector<pair<string, string> > > resultsComplex;

    XMLElement * elem = this->element->FirstChildElement();
    if ( elem != NULL )
    {
        std::string elTag = XmlElementReader::getName(elem);
        if ( elTag == "p" ) // SimpleTypes
        {
            // Parse Simple Parameters
            resultsSimple = getParametersSimple(elem);
        }
        if ( elTag == "item" ) // ComplexTypes
        {
            resultsComplex = getParametersComplex(elem);
        }
    }

    this->complexValues = resultsComplex;
    this->simpleValues = resultsSimple;
}

vector<pair<string, string> > GMCParameterListValue::getSimpleValues()
{
    return this->simpleValues;
}

vector< vector<pair<string, string> > > GMCParameterListValue::getComplexValues()
{
    return this->complexValues;
}
