#include <gtest/gtest.h>
#include <memory>

#include "Includes/InternalTypes/TDDTable.h"

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


using namespace std;
using namespace InternalTypes;


TEST(CSVParser, ParseTDDTable)
{
    TDDTable * table = new TDDTable(dir + "UT/TestFiles/CSV/tdd.csv");
    auto hertz = table->getHertzList();
    auto parameters = table->getParameterRows();

    delete table;

}
