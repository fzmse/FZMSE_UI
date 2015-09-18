#pragma once

#include <vector>
#include <string>

#include "CSVRow.h"

class CSVTable
{
public:
    CSVTable(std::vector<std::string> rawData);
    ~CSVTable();
    CSVRow * getRow(int index);
    std::vector<CSVRow*> getRows();
    std::string getCell(int row, int column);
    std::string toString();
protected:
    std::vector<CSVRow *> buildRows(std::vector<std::string> rawData);

    std::vector<CSVRow*> rows;
    std::vector<std::string> rawData;
};
