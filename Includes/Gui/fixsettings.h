#pragma once

#include <string>

class FixSetting
{
private:
    std::string fixFilePath;
    bool toBeSorted;

public:
    FixSetting();
    FixSetting(std::string path, bool sort);
    std::string getPath();
    bool isToBeSorted();
};
