#pragma once

#include <vector>
#include <string>

#include "Utilities/UtilStrings.hpp"

class CSVRow
{
public:
    CSVRow(std::string rowString);
    std::string getCell(int index);
    std::vector<std::string> getCells();
protected:
    std::vector<std::string> buildCells(std::string raw);
    std::vector<std::string> cells;

    std::string rowData;
};
