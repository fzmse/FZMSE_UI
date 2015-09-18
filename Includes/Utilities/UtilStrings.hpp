#pragma once

#include <string>
#include <vector>

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

inline std::vector<std::string> strSplit(std::string text, std::string delimiter)
{
    std::vector<std::string> results;
    std::string s = text;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        results.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    return results;
}
