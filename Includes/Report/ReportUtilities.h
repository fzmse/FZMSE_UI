#pragma once

#include <qbytearray.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qstring.h>

#include <string>
#include <vector>

#include "ReportEntry.h"

class ReportUtilities
{
public:
    static QByteArray getLogoData();
    static QString getReportTemplate();

    static bool saveLogo(std::string filename);
    static bool generateReport( std::vector<ReportEntry> entries, std::string filename,
                                std::string oldPDDBName, std::string newPDDBName,
                                std::string oldGMCName, std::string newGMCName, std::string pddbRelease);
};
