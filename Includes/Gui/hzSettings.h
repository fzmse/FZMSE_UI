#pragma once
#include <string>
#include "InternalTypes/gmcdocument.h"

#include "Csv/CSVTable.h"

class HzSettings
{
private:
    std::string path;
    std::string cellType;
    std::string hz;
    std::string frameConf;
    std::string specSubfConf;


public:
    HzSettings();
    HzSettings(std::string path, std::string cellType);

    std::string getPath() const;
    void setPath(const std::string &value);
    std::string getCellType() const;
    void setCellType(const std::string &value);
    std::string getHz() const;
    void setHz(const std::string &value);
    std::string getFrameConf() const;
    void setFrameConf(const std::string &value);
    std::string getSpecSubfConf() const;
    void setSpecSubfConf(const std::string &value);
};
