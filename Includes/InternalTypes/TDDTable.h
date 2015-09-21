#pragma once

#include "Csv/CSVParser.h"
#include "algorithm"

namespace InternalTypes
{
    class TDDTable : public CSVTable
    {
    public:
        TDDTable( std::string filename );
        std::vector<std::string> getHertzList();
        std::vector<CSVRow*> getParameterRows();
        std::vector<std::string> getFrameConf();
        std::vector<std::string> getSpecSubfConf();

        CSVRow * getParameterRowByName(std::string s);

    protected:

        std::vector<CSVRow *> resolveParameterRows();
        std::vector<std::string> resolveHertzList();

        std::vector<std::string> resolveFrameConf();
        std::vector<std::string> resolveSpecSubfConf();

        std::vector<std::string> hertzList;
        std::vector<std::string> frameConf;
        std::vector<std::string> specSubfConf;
        std::vector<CSVRow*> parameterRows;
    };
}
