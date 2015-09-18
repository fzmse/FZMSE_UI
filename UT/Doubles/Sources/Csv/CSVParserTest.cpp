#include <gtest/gtest.h>
#include <memory>

#include "Includes/Csv/CSVParser.h"

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


using namespace std;


TEST(CSVParser, BuildCSVTable)
{
    CSVTable * table = CSVParser::createTableFrom(dir + "UT/TestFiles/CSV/fdd_indoor.csv");
    delete table;
}
