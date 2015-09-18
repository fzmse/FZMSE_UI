#pragma once

#include "Csv/CSVParser.h"

namespace InternalTypes
{
    class FDDOutdoorTable : public CSVTable
    {
    public:
        FDDOutdoorTable( std::string filename );
        std::vector<std::string> getHertzList();
        std::vector<CSVRow*> getParameterRows();

    protected:

        std::vector<CSVRow *> resolveParameterRows();
        std::vector<std::string> resolveHertzList();

        std::vector<std::string> hertzList;
        std::vector<CSVRow*> parameterRows;
    };
}
