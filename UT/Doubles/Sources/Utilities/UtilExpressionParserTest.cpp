
#include <gtest/gtest.h>
#include "TinyXML/tinyxml2.h"

using namespace tinyxml2;
using namespace std;


#include "Includes/Utilities/UtilExpressionParser.hpp"

static std::string dir = "D:/Projects/FZMSE/FZMSE/";


TEST(UtilExpressionParser, CheckIfUIValueWorks)
{
    std::string result = extractAnswerFromExpression("= UI_value", "77");

    cout << result << endl;
}


TEST(UtilExpressionParser, CheckUIValueError)
{
    std::string result = extractAnswerFromExpression("= UI_value", "77/66/66/77/88");

    cout << result << endl;
}
