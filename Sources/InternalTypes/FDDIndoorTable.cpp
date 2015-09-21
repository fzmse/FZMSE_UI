#include "Includes/InternalTypes/FDDIndoorTable.h"

using namespace InternalTypes;
using namespace std;

FDDIndoorTable::FDDIndoorTable( std::string filename )
    : CSVTable(CSVParser::readFile(filename))
{
    this->hertzList = resolveHertzList();

    this->parameterRows = resolveParameterRows();
}

std::vector<std::string> FDDIndoorTable::getHertzList()
{
    return this->hertzList;
}

std::vector<CSVRow*> FDDIndoorTable::getParameterRows()
{
    return this->parameterRows;
}


std::vector<CSVRow *> FDDIndoorTable::resolveParameterRows()
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

std::vector<std::string> FDDIndoorTable::resolveHertzList()
{
    vector<string> hertzList;
    CSVRow * r = this->getRowByColValue(0, "dlChBw");
    if ( r != NULL )
    {
        for ( int i = 1; i < r->getCells().size(); i ++ )
            hertzList.push_back(r->getCell(i));
    }

    return hertzList;
}
CSVRow * FDDIndoorTable::getParameterRowByName(std::string s)
{
    for ( vector<CSVRow*>::iterator it = this->rows.begin(); it != this->rows.end(); it ++)
    {
        if ( (*it)->getCell(0) == s )
            return *it;
    }
    return NULL;
}
