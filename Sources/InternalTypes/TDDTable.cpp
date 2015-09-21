#include "Includes/InternalTypes/TDDTable.h"

using namespace InternalTypes;
using namespace std;

TDDTable::TDDTable( std::string filename )
    : CSVTable(CSVParser::readFile(filename))
{
    this->hertzList = resolveHertzList();

    this->parameterRows = resolveParameterRows();

    this->frameConf = resolveFrameConf();

    this->specSubfConf = resolveSpecSubfConf();
}

std::vector<std::string> TDDTable::getHertzList()
{
    return this->hertzList;
}

std::vector<CSVRow*> TDDTable::getParameterRows()
{
    return this->parameterRows;
}


std::vector<CSVRow *> TDDTable::resolveParameterRows()
{
    vector<CSVRow*> parameterRows;

    for ( vector<CSVRow*>::iterator it = this->rows.begin(); it != this->rows.end(); it ++)
    {
        CSVRow * r = *it;
        std::string firstCell = r->getCell(0);
        if ( firstCell != "" && firstCell.find(' ') == string::npos )
            parameterRows.push_back(r);
    }

    return parameterRows;
}

std::vector<std::string> TDDTable::resolveHertzList()
{
    vector<string> hertzList;
    CSVRow * r = this->getRowByColValue(0, "chBw (MHz)");
    if ( r != NULL )
    {
        for ( int i = 1; i < r->getCells().size(); i ++ )
        {
            bool found = false;
            for ( vector<string>::iterator it = hertzList.begin(); it != hertzList.end(); it ++)
                if ( (*it) == r->getCell(i))
                    found = true;

            if ( found == false )
                hertzList.push_back(r->getCell(i));
        }
    }

    std::sort(hertzList.begin(), hertzList.end());
    return hertzList;
}


std::vector<std::string> TDDTable::getFrameConf()
{
    return this->frameConf;
}

std::vector<std::string> TDDTable::getSpecSubfConf()
{
    return this->specSubfConf;
}

std::vector<std::string> TDDTable::resolveFrameConf()
{
    vector<string> hertzList;
    CSVRow * r = this->getRowByColValue(0, "tddFrameConf");
    if ( r != NULL )
    {
        for ( int i = 1; i < r->getCells().size(); i ++ )
        {
            bool found = false;
            for ( vector<string>::iterator it = hertzList.begin(); it != hertzList.end(); it ++)
                if ( (*it) == r->getCell(i))
                    found = true;

            if ( found == false )
                hertzList.push_back(r->getCell(i));
        }
    }

    std::sort(hertzList.begin(), hertzList.end());
    return hertzList;
}

std::vector<std::string> TDDTable::resolveSpecSubfConf()
{
    vector<string> hertzList;
    CSVRow * r = this->getRowByColValue(0, "tddSpecSubfConf");
    if ( r != NULL )
    {
        for ( int i = 1; i < r->getCells().size(); i ++ )
        {
            bool found = false;
            for ( vector<string>::iterator it = hertzList.begin(); it != hertzList.end(); it ++)
                if ( (*it) == r->getCell(i))
                    found = true;

            if ( found == false )
                hertzList.push_back(r->getCell(i));
        }
    }

    std::sort(hertzList.begin(), hertzList.end());
    return hertzList;
}

CSVRow * TDDTable::getParameterRowByName(std::string s)
{
    for ( vector<CSVRow*>::iterator it = this->rows.begin(); it != this->rows.end(); it ++)
    {
        if ( (*it)->getCell(0) == s )
            return *it;
    }
    return NULL;
}
