#include "Includes/InternalTypes/FDDOutdoorTable.h"

using namespace InternalTypes;
using namespace std;

FDDOutdoorTable::FDDOutdoorTable( std::string filename )
    : CSVTable(CSVParser::readFile(filename))
{
    this->hertzList = resolveHertzList();

    this->parameterRows = resolveParameterRows();
}

std::vector<std::string> FDDOutdoorTable::getHertzList()
{
    return this->hertzList;
}

std::vector<CSVRow*> FDDOutdoorTable::getParameterRows()
{
    return this->parameterRows;
}


std::vector<CSVRow *> FDDOutdoorTable::resolveParameterRows()
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

std::vector<std::string> FDDOutdoorTable::resolveHertzList()
{
    vector<string> hertzList;
    CSVRow * r = this->getRowByColValue(3, "dlChBw");
    if ( r != NULL )
    {
        for ( int i = 4; i < r->getCells().size(); i ++ )
            hertzList.push_back(r->getCell(i));
    }

    return hertzList;
}

CSVRow * FDDOutdoorTable::getParameterRowByName(std::string s)
{
    for ( vector<CSVRow*>::iterator it = this->rows.begin(); it != this->rows.end(); it ++)
    {
        if ( (*it)->getCell(3) == s )
            return *it;
    }
    return NULL;
}
