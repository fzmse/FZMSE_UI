
#include <QMainWindow>
#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"
#include "Xml/XmlWrapper.h"
#include "Includes/Gui/resultItemModel.h"
#include "Includes/InternalTypes/PDDBManagedObject.h"
#include "InternalTypes/pddbdocument.h"
#include "Includes/Gui/xmlhighlighter.h"

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

class appGUI : public QMainWindow
{
    Q_OBJECT

public:
    appGUI();
    void createLoadPathActions();
    std::vector<QString> parseXmlByEndLine(std::string XML);
    void colorTextDifferences();
    void setLabels(QString desc);

private slots:
    void loadPathToDoc(const QString&);
    void save();
    void generateRaport();
    void help();
    void clean();
    void comparePDDB();
    void showSelectedPDDBResult();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    void createPDDBResultDock();
    void createPDDBDescriptionDock();
    void createPDDBResultView();


    QLabel * upperLabel;
    QLabel * bottomLabel;
    QTextEdit * upperTextEdit;
    QTextEdit * bottomTextEdit;
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

    QDockWidget * PDDBResultDock;
    QTreeView * PDDBResultView;
    resultItemModel * PDDBResultModel;

    shared_ptr<XMLHighlighter> xmlHighlighterUp;
    shared_ptr<XMLHighlighter> xmlHighlighterBottom;

    XmlReader * reader;
    XmlElementReader * elementReader;

    std::string oldPDDBPath;
    std::string newPDDBPath;
    std::string oldGMCPath;
    std::string newGMCPath;

    shared_ptr<InternalTypes::PDDBDocument> oldPDDBdoc;
    shared_ptr<InternalTypes::PDDBDocument> newPDDBdoc;
    shared_ptr<InternalTypes::PDDBDocument> oldGMCdoc;
    shared_ptr<InternalTypes::PDDBDocument> newGMCdoc;

    std::string getFileName(std::string path);

};

