#include <string>

class ReportSetting
{
private:
    std::string templatePath;
    bool toBeSorted;

public:
    ReportSetting(std::string path, bool sort);
    std::string getPath();
    bool isToBeSorted();
};
