
#include "Gui/fixsettings.h"


FixSetting::FixSetting()
{
    fixFilePath = "";
    toBeSorted = false;
    toBeSortedPara = false;
}

FixSetting::FixSetting(std::string path, bool sort, bool sortPara)
{
    fixFilePath = path;
    toBeSorted = sort;
    toBeSortedPara = sortPara;
}

std::string FixSetting::getPath()
{
    return fixFilePath;
}

bool FixSetting::isToBeSorted()
{
    return toBeSorted;
}

bool FixSetting::isToBeSortedPara()
{
    return toBeSortedPara;
}


