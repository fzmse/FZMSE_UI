#pragma once

class appGUI;

#include <QMainWindow>
#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"
#include "Xml/XmlWrapper.h"
#include "Includes/Gui/resultItemModel.h"
#include "Includes/InternalTypes/PDDBManagedObject.h"
#include "Includes/Gui/gmcresultitemmodel.h"
#include "Includes/InternalTypes/GMCManagedObject.h"
#include "InternalTypes/pddbdocument.h"
#include "InternalTypes/gmcdocument.h"
#include "InternalTypes/gmcwriter.h"
#include "Includes/Gui/xmlhighlighter.h"
#include "Includes/InternalTypes/PDDBManagedObjectCompareResult.h"
#include "Includes/Report/ReportUtilities.h"
#include <QtWidgets>
#include <cstring>

class QLabel;
class QAction;
class QListWidget;
class QListView;
class QMenu;
class QGroupBox;
class QPushButton;
class QTextEdit;
class QTreeView;
class QStandardItemModel;
class QStandardItem;
class QTextCursor;
class QHBoxLayout;
class QVBoxLayout;
class QPoint;


class appGUI : public QMainWindow
{
    Q_OBJECT

public:
    appGUI();
    void createLoadPathActions();
    void colorTextDifferences();
    void setLabels(QString desc, bool PDDB);
    void setLabel(QLabel * label, std::string text);
    void refreshGMCView();
    void changeUserInteraction(gmcResultItem * item);
    std::vector<QString> parseXmlByEndLine(std::string XML);
    vector<InternalTypes::GMCAction> * getActions();

private slots:
    void loadPathToDoc(const QString &);
    void save();
    void help();
    void clean();
    void compare();
    void showSelectedPDDBResult();
    void showSelectedGMCResult();
    void onGMCRClick(const QPoint &);

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();

    void createPDDBResultDock();
    void createPDDBDescriptionDock();
    void createPDDBResultView();

    void createGMCResultDock();
    void createGMCDescriptionDock();
    void createGMCResultView();

    void createPDDBTextDock();

    void createGMCTextDock();

    void comparePDDB();
    void printDiff(resultItem * r);

    QVBoxLayout * verCentralLayout;
    QWidget * centralWid;

    QTextEdit * oldPDDBTextEdit;
    QTextEdit * newPDDBTextEdit;

    QTextEdit * oldGMCTextEdit;
    QTextEdit * newGMCTextEdit;

    QGroupBox * centralGroup;
    QGroupBox * centralButtonSubGroup;

    QMenu * fileMenu;
    QMenu * helpMenu;
    QMenu * viewMenu;

    QToolBar * fileToolBar;

    QAction * openOldPDDBAct;
    QAction * openNewPDDBAct;
    QAction * openOldGMCAct;
    QAction * saveFileAct;
    QAction * displayHelpAct;
    QAction * addToGMC;
    QAction * delFromGMC;


    QPushButton * clearListsBut;
    QPushButton * compareBut;

    QDockWidget * PDDBDescriptionDock;
    QDockWidget * PDDBResultViewDock;
    QDockWidget * GMCResultViewDock;
    QDockWidget * PDDBTextDock;
    QDockWidget * GMCTextViewDock;

    QDockWidget * PDDBResultDock;
    QTreeView * PDDBResultView;
    resultItemModel * PDDBResultModel;

    QMenu* contextMenu;

    QDockWidget * GMCResultDock;
    QTreeView * GMCResultView;
    gmcResultItemModel * GMCResultModel;

    QLabel * oldPDDBLabel;
    QLabel * newPDDBLabel;
    QLabel * oldGMCLabel;
    QLabel * newGMCLabel;

    shared_ptr<XMLHighlighter> xmlHighlighterOldPDDB;
    shared_ptr<XMLHighlighter> xmlHighlighterNewPDDB;
    shared_ptr<XMLHighlighter> xmlHighlighterOldGMC;
    shared_ptr<XMLHighlighter> xmlHighlighterNewGMC;

    XmlReader * reader;
    XmlElementReader * elementReader;

    std::string oldPDDBPath;
    std::string newPDDBPath;
    std::string oldGMCPath;

    shared_ptr<InternalTypes::PDDBDocument> oldPDDBdoc;
    shared_ptr<InternalTypes::PDDBDocument> newPDDBdoc;
    shared_ptr<InternalTypes::GMCDocument> oldGMCdoc;
    shared_ptr<InternalTypes::GMCDocument> newGMCdoc;

    std::string getFileName(std::string path);
    std::vector<InternalTypes::PDDBManagedObjectCompareResult> differences;
    vector<InternalTypes::GMCAction> actions;
};

