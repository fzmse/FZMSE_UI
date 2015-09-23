#pragma once

#include <string>

class FixSetting
{
private:
    std::string fixFilePath;
    bool toBeSorted;
    bool toBeSortedPara;

public:
    FixSetting();
    FixSetting(std::string path, bool sort, bool sortPara);
    std::string getPath();
    bool isToBeSorted();
    bool isToBeSortedPara();
};
