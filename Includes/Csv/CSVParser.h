#pragma once

#include <vector>
#include <string>

#include <iostream>
#include <fstream>

#include "CSVTable.h"

class CSVParser
{
public:
    static CSVTable * createTableFrom(std::string filename);
    static std::vector<std::string> readFile(std::string filename);

};
