#pragma once

#include "Csv/CSVParser.h"

namespace InternalTypes
{
    class FDDIndoorTable : public CSVTable
    {
    public:
        FDDIndoorTable( std::string filename );
        std::vector<std::string> getHertzList();
        std::vector<CSVRow*> getParameterRows();

    protected:

        std::vector<CSVRow *> resolveParameterRows();
        std::vector<std::string> resolveHertzList();

        std::vector<std::string> hertzList;
        std::vector<CSVRow*> parameterRows;
    };
}
