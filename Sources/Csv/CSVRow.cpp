#include "Includes/Csv/CSVRow.h"



CSVRow::CSVRow(std::string rowString)
{
    this->rowData = rowString;
    this->cells = buildCells(rowString);
}

std::string CSVRow::getCell(int index)
{
    std::string result = "";
    if ( index < this->cells.size() )
        result = this->cells[index];
    return result;
}

std::vector<std::string> CSVRow::buildCells(std::string raw)
{
    std::vector<std::string> cellsVector = strSplit(raw, ";");
    return cellsVector;
}


std::vector<std::string> CSVRow::getCells()
{
    return this->cells;
}
