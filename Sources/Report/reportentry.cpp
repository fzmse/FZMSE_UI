
#include "Report/ReportEntry.h"

ReportEntry::ReportEntry(EntryType e, std::string parameter, std::string reason, std::string oldValue, std::string newValue)
{
    this->entryType = e;
    this->parameter = parameter;
    this->reason = reason;
    this->oldValue = oldValue;
    this->newValue = newValue;
}

ReportEntry::EntryType ReportEntry::getEntryType()
{
    return this->entryType;
}

std::string ReportEntry::getParameter()
{
    return this->parameter;
}

std::string ReportEntry::getReason()
{
    return this->reason;
}

std::string ReportEntry::getOldValue()
{
    return this->oldValue;
}

std::string ReportEntry::getNewValue()
{
    return this->newValue;
}
