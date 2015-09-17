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
#include "Includes/Gui/helpdialog.h"
#include "Report/reportsettings.h"
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
class QSettings;

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
    void createSaveDialog();
    void about();
    void clean();
    void compare();
    void showSelectedPDDBResult();
    void showSelectedGMCResult(QModelIndex);
    void onGMCRClick(const QPoint &);
    void setPDDBHint(QModelIndex);
    void setGMCHint(QModelIndex);
    void loadPathToDoc(const QString &);
    void setCurrDist(QModelIndex);
    void setDistToAction();
    void closeDistNameDialog();
    void acceptReportSettings();
    void cancelReportSettings();
    void loadTemplatePath();
    void setToBeSort(bool);
    void radioNewRaport(bool);

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

    void setConnections();

    void save();

    void comparePDDB();
    void printDiff(resultItem * r);

    void createDistNameDialog();

    QVBoxLayout * verCentralLayout;
    QWidget * centralWid;

    QTextEdit * oldPDDBTextEdit;
    QTextEdit * newPDDBTextEdit;

    QTextEdit * oldGMCTextEdit;
    QTextEdit * newGMCTextEdit;

    QGroupBox * centralGroup;
    QGroupBox * centralButtonSubGroup;

    QMenu * fileMenu;
    QMenu * aboutMenu;
    QMenu * viewMenu;

    QToolBar * fileToolBar;

    QAction * openOldPDDBAct;
    QAction * openNewPDDBAct;
    QAction * openOldGMCAct;
    QAction * saveFileAct;
    QAction * displayAboutAct;
    QAction * addToGMC;
    QAction * delFromGMC;
    QAction * setDistName;
    QAction * selectClicked;

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

    QDialog * dialogList;
    QStringListModel * distListModel;
    QString currDist;
    InternalTypes::GMCAction * currAction;
    QListView * distListView;

    QDialog * saveDialog;
    std::string templatePath;
    bool toBeSorted;
    ReportSetting reportSettings;
    QRadioButton * genNewRadio;
    QRadioButton * genFromTamplateRadio;
    QLineEdit * pathLine;
    QPushButton * loadPathButton;
    QPushButton * acceptButton;
    QPushButton * cancelButton;

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

