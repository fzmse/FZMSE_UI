#include "Includes/Csv/CSVParser.h"

std::vector<std::string> CSVParser::readFile(std::string filename)
{
    std::vector<std::string> lines;

    std::ifstream file(filename);
    std::string line = "";
    while(std::getline(file, line)) {
         //Do with temp
        lines.push_back(line);
    }

    return lines;
}


CSVTable * CSVParser::createTableFrom(std::string filename)
{
    return new CSVTable(readFile(filename));
}


