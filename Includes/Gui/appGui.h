#ifndef APPGUI_H
#define APPGUI_H

#include <QMainWindow>
#include <QMdiArea>
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

class appGUI : public QMainWindow
{
    Q_OBJECT

public:
    appGUI();


private slots:
    void openOldPDDB();
    void openNewPDDB();
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
    QAction * saveFileAct;
    QAction * generateRaportAct;
    QAction * displayHelpAct;
    QPushButton * clearListsBut;
    QPushButton * compareBut;

    XmlReader * reader;
    XmlElementReader * elementReader;
};

#endif // APPGUI_H
