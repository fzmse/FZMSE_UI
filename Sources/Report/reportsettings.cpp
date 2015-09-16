#include "Report/reportsettings.h"


ReportSetting::ReportSetting()
{
    templatePath = "";
    toBeSorted = false;
}

ReportSetting::ReportSetting(std::string path, bool sort)
{
    templatePath = path;
    toBeSorted = sort;
}

std::string ReportSetting::getPath()
{
    return templatePath;
}

bool ReportSetting::isToBeSorted()
{
    return toBeSorted;
}


