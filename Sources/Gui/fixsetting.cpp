
#include "Gui/fixsettings.h"


FixSetting::FixSetting()
{
    fixFilePath = "";
    toBeSorted = false;
}

FixSetting::FixSetting(std::string path, bool sort)
{
    fixFilePath = path;
    toBeSorted = sort;
}

std::string FixSetting::getPath()
{
    return fixFilePath;
}

bool FixSetting::isToBeSorted()
{
    return toBeSorted;
}


