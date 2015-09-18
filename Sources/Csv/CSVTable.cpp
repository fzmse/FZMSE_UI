#include "Includes/Csv/CSVTable.h"

CSVTable::CSVTable(std::vector<std::string> rawData)
{
    this->rawData = rawData;

    this->rows = this->buildRows(rawData);

}

CSVTable::~CSVTable()
{
    for ( std::vector<CSVRow*>::iterator it = this->rows.begin(); it != this->rows.end(); it ++ )
    {
        if ( *it != NULL )
            delete *it;
        *it = NULL;
    }
}

CSVRow * CSVTable::getRow(int index)
{
    CSVRow * row = NULL;
    if ( index < this->rows.size() )
        row = this->rows[index];
    return row;
}

std::string CSVTable::getCell(int row, int column)
{
    std::string cellText = "";
    CSVRow * rowPointer = getRow(row);
    if ( rowPointer != NULL )
        cellText = rowPointer->getCell(column);
    return cellText;
}


std::vector<CSVRow *> CSVTable::buildRows(std::vector<std::string> rawData)
{
    std::vector<CSVRow*> rowVector;
    for ( std::vector<std::string>::iterator it = rawData.begin(); it != rawData.end(); it ++ )
        rowVector.push_back(new CSVRow(*it));
    return rowVector;
}

 std::vector<CSVRow*> CSVTable::getRows()
 {
    return this->rows;
 }

 std::string CSVTable::toString()
 {
    std::string result = "";
    for ( int i = 0; i < this->rows.size(); i ++ )
    {
        for ( int u = 0; u < this->rows[i]->getCells().size(); u ++ )
        {
            result += this->getCell(i, u);
            result += "\t";
        }
        result += "\n";
    }
    return result;
 }
