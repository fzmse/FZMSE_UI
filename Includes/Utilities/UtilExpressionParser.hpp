#pragma once

#include <string>
#include "ExpressionParser/parser.h"
#include "UtilStrings.hpp"


inline std::string extractAnswerFromExpression(std::string expression, std::string ui_value)
{
    Parser p;
    std::string s = expression;
    if ( s.size() >= 2 )
    {
        s = s.substr(2, s.size() - 2);
        s = strReplace(s, "UI_value", ui_value);
    }
    const char * parseResult = p.parse(s.c_str());
    std::string result = parseResult;

    if ( result.size() >= 3 )
    {
        if ( result.substr(0, 3) != "Ans" )
            return "";
    }
    else
        return "";

    result = strReplace(result, "Ans = ", ""); // ERROR!
    return result;

}
