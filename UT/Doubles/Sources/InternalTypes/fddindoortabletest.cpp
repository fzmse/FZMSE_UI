
#include <gtest/gtest.h>
#include <memory>

#include "Includes/InternalTypes/FDDIndoorTable.h"
#include "Includes/InternalTypes/FDDOutdoorTable.h"

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


using namespace std;
using namespace InternalTypes;


TEST(CSVParser, ParseIndoorTable)
{
    FDDOutdoorTable * table = new FDDOutdoorTable(dir + "UT/TestFiles/CSV/fdd_outdoor.csv");
    auto hertz = table->getHertzList();
    auto parameters = table->getParameterRows();

    delete table;

}
