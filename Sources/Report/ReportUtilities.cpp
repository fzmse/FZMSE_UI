#include "Includes/Report/ReportUtilities.h"

using namespace std;

QByteArray ReportUtilities::getLogoData()
{
    QFile mFile("://report/logo.png");
    if ( mFile.open(QFile::ReadOnly) )
    {
        QByteArray imgData = mFile.readAll();
        mFile.close();
        return imgData;
    }

}

QString ReportUtilities::getReportTemplate()
{
    QFile mFile(":/report/reptemp.html");
    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
            return "";
        }

    QTextStream in(&mFile);
    QString mText = in.readAll();

    mFile.close();

    return mText;
}

bool ReportUtilities::saveLogo(string filename)
{
    QFile outFile(QString::fromStdString(filename));
    if ( outFile.open(QIODevice::WriteOnly  ) )
    {
        auto data = getLogoData();
        outFile.write(data);
        outFile.flush();

        outFile.close();

        auto str = outFile.errorString();
        return true;
    }
    return false;
}

inline QString generateAddedHTML(vector<ReportEntry> entries, bool withHeader = true)
{
    QString result = "";
    std::string header = "";
    if ( withHeader )
    {
        header += "\n<div class=\"div_spacer20px\"></div><span class=\"table_header\">Added</span>";
        header += "\n<div class=\"div_spacer5px\"></div> ";
        header +=  "\n<div  style=\"display:table; border: 2px solid black; width: 100%;\">";

        header +=      "\n<!-- Header table definition -->";
        header +=      "\n<div style=\"width: 35%; word-wrap: break-word; display:table-cell;  \">";
        header +=          "\n<span class=\"table_entryHeader\">Parameter</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"width: 25%; word-wrap: break-word;  display:table-cell;  border-left: 2px solid black;\">";
        header +=          "\n<span class=\"table_entryHeader\">Default value</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"width: 40%; word-wrap: break-word;  display:table-cell;  border-left: 2px solid black; \">";
        header +=          "\n<span class=\"table_entryHeader\">Reason</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"clear: left;\"></div>";

        header += "\n</div>\n\n";
    }


    for ( vector<ReportEntry>::iterator it = entries.begin(); it != entries.end(); it ++  )
    {
        ReportEntry e = *it;
        if ( e.getEntryType() == ReportEntry::Add )
        {
            std::string entryTemplate = "";
            entryTemplate += "\n\n<div style=\"display:table; border-left: 1px solid black;  border-right: 1px solid black;  border-bottom: 1px solid black;  width: 100%; \" >";
            entryTemplate +=         "\n<div style=\" width: 35%; height: 100%; word-wrap: break-word; display:table-cell;\">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getParameter() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\" width: 25%; height: 100%; word-wrap: break-word;  display:table-cell;  border-left: 1px solid black;   \">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getNewValue() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\" width: 40%; height: 100%; word-wrap: break-word; display:table-cell;  border-left: 1px solid black;  \">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getReason() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\"clear: left;\"></div>";
            entryTemplate +=    "\n</div>\n\n";

            result += QString::fromStdString(entryTemplate);
        }
    }

    if ( result.length() > 0 )
    {
        return QString::fromStdString(header) + result + "\n";
    }

    return "";
}

inline QString generateRemovedHTML(vector<ReportEntry> entries, bool withHeader = true)
{
    QString result = "";
    std::string header = "";
    if ( withHeader )
    {
        header += "\n<div class=\"div_spacer20px\"></div><span class=\"table_header\">Removed</span>";
        header += "\n<div class=\"div_spacer5px\"></div> ";
        header +=  "\n<div  style=\"display:table; border: 2px solid black; width: 100%;\">";

        header +=      "\n<!-- Header table definition -->";
        header +=      "\n<div style=\"width: 60%; word-wrap: break-word; display:table-cell;  \">";
        header +=          "\n<span class=\"table_entryHeader\">Parameter</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"width: 40%; word-wrap: break-word;  display:table-cell;  border-left: 2px solid black; \">";
        header +=          "\n<span class=\"table_entryHeader\">Reason</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"clear: left;\"></div>";

        header += "\n</div>\n\n";
    }

    for ( vector<ReportEntry>::iterator it = entries.begin(); it != entries.end(); it ++  )
    {
        ReportEntry e = *it;
        if ( e.getEntryType() == ReportEntry::Remove )
        {
            std::string entryTemplate = "";
            entryTemplate += "\n\n<div style=\"display:table; border-left: 1px solid black;  border-right: 1px solid black;  border-bottom: 1px solid black;  width: 100%; \" >";
            entryTemplate +=         "\n<div style=\" width: 60%; height: 100%; word-wrap: break-word; display:table-cell;\">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getParameter() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\" width: 40%; height: 100%; word-wrap: break-word; display:table-cell;  border-left: 1px solid black;  \">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getReason() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\"clear: left;\"></div>";
            entryTemplate +=    "\n</div>\n\n";

            result += QString::fromStdString(entryTemplate);
        }
    }

    if ( result.length() > 0 )
    {
        return QString::fromStdString(header) + result + "\n";
    }

    return "";
}

inline QString generateModifiedHTML(vector<ReportEntry> entries, bool withHeader = true)
{
    QString result = "";
    std::string header = "";
    if ( withHeader )
    {
        header += "\n<div class=\"div_spacer20px\"></div><span class=\"table_header\">Modified</span>";
        header += "\n<div class=\"div_spacer5px\"></div> ";
        header +=  "\n<div  style=\"display:table; border: 2px solid black; width: 100%;\">";

        header +=      "\n<!-- Header table definition -->";
        header +=      "\n<div style=\"width: 35%; word-wrap: break-word; display:table-cell;  \">";
        header +=          "\n<span class=\"table_entryHeader\">Parameter</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"width: 20%; word-wrap: break-word;  display:table-cell;  border-left: 2px solid black;\">";
        header +=          "\n<span class=\"table_entryHeader\">Old value</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"width: 20%; word-wrap: break-word;  display:table-cell;  border-left: 2px solid black;\">";
        header +=          "\n<span class=\"table_entryHeader\">New value</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"width: 25%; word-wrap: break-word;  display:table-cell;  border-left: 2px solid black; \">";
        header +=          "\n<span class=\"table_entryHeader\">Reason</span>";
        header +=      "\n</div>";
        header +=      "\n<div style=\"clear: left;\"></div>";

        header += "\n</div>\n\n";
    }


    for ( vector<ReportEntry>::iterator it = entries.begin(); it != entries.end(); it ++  )
    {
        ReportEntry e = *it;
        if ( e.getEntryType() == ReportEntry::Modify )
        {
            std::string entryTemplate = "";
            entryTemplate += "\n\n<div style=\"display:table; border-left: 1px solid black;  border-right: 1px solid black;  border-bottom: 1px solid black;  width: 100%; \" >";
            entryTemplate +=         "\n<div style=\" width: 35%; height: 100%; word-wrap: break-word; display:table-cell;\">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getParameter() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\" width: 20%; height: 100%; word-wrap: break-word;  display:table-cell;  border-left: 1px solid black;   \">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getOldValue() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\" width: 20%; height: 100%; word-wrap: break-word;  display:table-cell;  border-left: 1px solid black;   \">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getNewValue() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\" width: 25%; height: 100%; word-wrap: break-word; display:table-cell;  border-left: 1px solid black;  \">";
            entryTemplate +=             "\n<span class=\"table_entry\">" + e.getReason() + "</span>";
            entryTemplate +=         "\n</div>";
            entryTemplate +=         "\n<div style=\"clear: left;\"></div>";
            entryTemplate +=    "\n</div>\n\n";

            result += QString::fromStdString(entryTemplate);
        }
    }

    if ( result.length() > 0 )
    {
        return QString::fromStdString(header) + result + "\n";
    }

    return "";
}


bool ReportUtilities::generateReport( vector<ReportEntry> entries, string filename, string oldPDDBName, string newPDDBName, string oldGMCName, string newGMCName, string pddbRelease)
{
    QFile outFile(QString::fromStdString(filename));
    if ( outFile.open(QIODevice::WriteOnly | QIODevice::Text ) )
    {

        auto transformedText = getReportTemplate();

        transformedText = transformedText.replace("<@@@OLD_PDDB_NAME@@@>", QString::fromStdString(oldPDDBName));
        transformedText = transformedText.replace("<@@@NEW_PDDB_NAME@@@>", QString::fromStdString(newPDDBName));
        transformedText = transformedText.replace("<@@@OLD_GMC_NAME@@@>", QString::fromStdString(oldGMCName));
        transformedText = transformedText.replace("<@@@NEW_GMC_NAME@@@>", QString::fromStdString(newGMCName));

        transformedText = transformedText.replace("<@@@NEW_PDDB_RELEASE@@@>", QString::fromStdString(pddbRelease));
        transformedText = transformedText.replace("<@@@NEW_PDDB_RELEASE@@@>", QString::fromStdString(pddbRelease));

        QString addTableContentHTML = generateAddedHTML(entries);
        transformedText = transformedText.replace("<@@@ADDED_ENTRIES@@@>", addTableContentHTML);

        QString modifyTableContentHTML = generateModifiedHTML(entries);
        transformedText = transformedText.replace("<@@@MODIFIED_ENTRIES@@@>", modifyTableContentHTML);

        QString removeTableContentHTML = generateRemovedHTML(entries);
        transformedText = transformedText.replace("<@@@REMOVED_ENTRIES@@@>", removeTableContentHTML);

        outFile.write(transformedText.toStdString().c_str());
        outFile.flush();

        outFile.close();

        auto str = outFile.errorString();
        return true;
    }
    return false;
}

inline int getIndexOfEndOfString(QString text, QString textToFind)
{
    int posDiffSection = text.indexOf(textToFind);
    if ( posDiffSection >= 0 )
        posDiffSection += textToFind.length();

    return posDiffSection;
}


bool ReportUtilities::generateReportFromExisting( std::string reportTemplate, vector<ReportEntry> entries, string filename, string oldPDDBName, string newPDDBName, string oldGMCName, string newGMCName, string pddbRelease)
{
    QFile outFile(QString::fromStdString(filename));
    if ( outFile.open(QIODevice::WriteOnly | QIODevice::Text ) )
    {

        auto transformedText = QString::fromStdString(reportTemplate);

        const QString DIFFERENCES_SECTION = "<div id=\"differences_section\">";
        const int DIFFERENCES_SECTION_END = getIndexOfEndOfString(transformedText, DIFFERENCES_SECTION);

        const QString END_TABLE_SECTION = "<div class=\"div_spacer40px\"></div>";

        const QString ADDED_SECTION = "<div class=\"div_spacer20px\"></div><span class=\"table_header\">Added</span>";
        const int ADDED_SECTION_END = getIndexOfEndOfString(transformedText, ADDED_SECTION);

        const QString MODIFIED_SECTION = "<div class=\"div_spacer20px\"></div><span class=\"table_header\">Modified</span>";
        const int MODIFIED_SECTION_END = getIndexOfEndOfString(transformedText, MODIFIED_SECTION);

        const QString REMOVED_SECTION = "<div class=\"div_spacer20px\"></div><span class=\"table_header\">Removed</span>";
        const int REMOVED_SECTION_END = getIndexOfEndOfString(transformedText, REMOVED_SECTION);

        bool addedSectionExists = (ADDED_SECTION_END > 0);
        bool modifiedSectionExists = (MODIFIED_SECTION_END > 0);
        bool removedSectionExists = (REMOVED_SECTION_END > 0);

        // ADD INSERT
        QString addTableContentHTML = generateAddedHTML(entries, !addedSectionExists);
        int posToInsertAdd = -1;
        if ( addedSectionExists == false )
            posToInsertAdd = DIFFERENCES_SECTION_END;
        else
        {
            if ( removedSectionExists )
                            posToInsertAdd = transformedText.indexOf(REMOVED_SECTION);
            else if ( modifiedSectionExists )
                posToInsertAdd = transformedText.indexOf(MODIFIED_SECTION);
            else
            {
                posToInsertAdd = transformedText.indexOf(END_TABLE_SECTION);
            }
        }
        transformedText.insert(posToInsertAdd, addTableContentHTML);
        if ( addTableContentHTML != "" )
            addedSectionExists = true;


        // MODIFY INSERT
        QString modifyTableContentHTML = generateModifiedHTML(entries, !modifiedSectionExists);
        int posToInsertModify = -1;
        posToInsertModify = transformedText.indexOf(END_TABLE_SECTION);
        transformedText.insert(posToInsertModify, modifyTableContentHTML);
        if ( modifyTableContentHTML != "" )
            modifiedSectionExists = true;


        // REMOVE INSERT
        QString removeTableContentHTML  = generateRemovedHTML(entries, !removedSectionExists);
        int posToInsertRemove = -1;
        if ( removedSectionExists == false)
        {
            if ( addedSectionExists == false )
                posToInsertRemove = DIFFERENCES_SECTION_END;
            else
            {
                if ( modifiedSectionExists )
                    posToInsertRemove = transformedText.indexOf(MODIFIED_SECTION);
                else
                    posToInsertRemove = transformedText.indexOf(END_TABLE_SECTION);
            }
        }
        else
        {
            if ( modifiedSectionExists == false )
                posToInsertRemove = transformedText.indexOf(END_TABLE_SECTION);
            else
                posToInsertRemove = transformedText.indexOf(MODIFIED_SECTION);

        }
        transformedText.insert(posToInsertRemove, removeTableContentHTML);
        if ( removeTableContentHTML != "" )
            removedSectionExists = true;


        outFile.write(transformedText.toStdString().c_str());
        outFile.flush();

        outFile.close();

        auto str = outFile.errorString();
        return true;
    }
    return false;
}




