
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
#include "Includes/Gui/xmlhighlighter.h"
#include "Includes/InternalTypes/PDDBManagedObjectCompareResult.h"

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

class appGUI : public QMainWindow
{
    Q_OBJECT

public:
    appGUI();
    void createLoadPathActions();
    void colorTextDifferences();
    void setLabels(QString desc, bool PDDB);
    void setLabel(QLabel * label, std::string text);
    std::vector<QString> parseXmlByEndLine(std::string XML);


private slots:
    void loadPathToDoc(const QString&);
    void save();
    void generateRaport();
    void help();
    void clean();
    void compare();
    void showSelectedPDDBResult();
    void showSelectedGMCResult();

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
    QMenu * raportMenu;
    QMenu * helpMenu;
    QMenu * viewMenu;

    QToolBar * fileToolBar;
    QToolBar * raportToolBar;

    QAction * openOldPDDBAct;
    QAction * openNewPDDBAct;
    QAction * openOldGMCAct;
    QAction * openNewGMCAct;
    QAction * saveFileAct;
    QAction * generateRaportAct;
    QAction * displayHelpAct;

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
    std::string newGMCPath;

    shared_ptr<InternalTypes::PDDBDocument> oldPDDBdoc;
    shared_ptr<InternalTypes::PDDBDocument> newPDDBdoc;
    shared_ptr<InternalTypes::GMCDocument> oldGMCdoc;
    shared_ptr<InternalTypes::GMCDocument> newGMCdoc;

    std::string getFileName(std::string path);
    std::vector<InternalTypes::PDDBManagedObjectCompareResult> differences;
};

