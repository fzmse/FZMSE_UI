#pragma once

#include <vector>
#include <string>

class ReportEntry
{
public:
    enum EntryType
    {
        Add, Modify, Remove
    };

    ReportEntry(EntryType e, std::string parameter, std::string reason, std::string oldValue = "", std::string newValue = "");

    EntryType getEntryType();

    std::string getParameter();
    std::string getReason();
    std::string getOldValue();
    std::string getNewValue();

protected:
    EntryType entryType;
    std::string parameter;
    std::string reason;
    std::string oldValue;
    std::string newValue;


};
