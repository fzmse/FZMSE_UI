#pragma once

#include <string>

inline std::string strReplace(std::string str, std::string old, std::string news)
{
    int currLength = str.length() + 1;
    while ( currLength > str.length() )
    {
        currLength = str.length();
        size_t start_pos = str.find(old);
        if(start_pos == std::string::npos)
            return str;
        str.replace(start_pos, old.length(), news);
    }
    return str;
}
