#include "Gui/hzSettings.h"

HzSettings::HzSettings()
{
    path = "";
    cellType = "";
    hz = "";
    frameConf = "";
    specSubfConf = "";
}

HzSettings::HzSettings(std::string file_path, std::string file_cellType)
{
    path = file_path;
    cellType = file_cellType;
}

std::string HzSettings::getPath() const
{
    return path;
}

void HzSettings::setPath(const std::string &value)
{
    path = value;
}

std::string HzSettings::getCellType() const
{
    return cellType;
}

void HzSettings::setCellType(const std::string &value)
{
    cellType = value;
}

std::string HzSettings::getHz() const
{
    return hz;
}

void HzSettings::setHz(const std::string &value)
{
    hz = value;
}

std::string HzSettings::getFrameConf() const
{
    return frameConf;
}

void HzSettings::setFrameConf(const std::string &value)
{
    frameConf = value;
}

std::string HzSettings::getSpecSubfConf() const
{
    return specSubfConf;
}

void HzSettings::setSpecSubfConf(const std::string &value)
{
    specSubfConf = value;
}
