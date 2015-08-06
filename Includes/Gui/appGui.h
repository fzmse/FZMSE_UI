
#include <QMainWindow>
#include "TinyXML/tinyxml2.h"
#include "Xml/XmlReader.h"
#include "Xml/XmlElementReader.h"
#include "Xml/XmlWrapper.h"

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

class appGUI : public QMainWindow
{
    Q_OBJECT

public:
    appGUI();


    void createLoadPathActions();
    std::vector<QString> parseXmlByEndLine(std::string XML);
private slots:
    void loadPathTo(const QString&);
    void save();
    void generateRaport();
    void help();
    void clean();
    void compare();
    void insertCompareResult(const QString &result);

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    void createDockWindows();

    enum { };

    QListWidget * upperListView;
    QListWidget * bottomListView;
    QListWidget * resultList;
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

    QTreeView * resultTreeView;
    QStandardItemModel * resultStandardModel;
    QStandardItemModel * subModel;

    QList<QString *> listOfLabels(const int& n);

    QList<QStandardItem *> listMOCTemplate(const QString& className);

    QList<QStandardItem *> subListResultTypes(const QString& name,
                                              const QString& count);

    QList<QStandardItem *> listResultType(const QString& name,
                                       const QString& added,
                                       const QString& removed,
                                       const QString& modified);

    XmlReader * reader;
    XmlElementReader * elementReader;

    std::string oldPDDBPath;
    std::string newPDDBPath;
    std::string oldGMCPath;
    std::string newGMCPath;

};

