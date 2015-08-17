#include <QtWidgets>
#include "Gui/appGui.h"
#include <cstring>

using namespace InternalTypes;
//using namespace tinyxml2;

appGUI::appGUI()
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    centralGroup = new QGroupBox;
    QVBoxLayout * centralLayout = new QVBoxLayout;

    upperListView = new QListWidget;
    bottomListView = new QListWidget;

    centralButtonSubGroup = new QGroupBox;
    QHBoxLayout * centralButtonLayout = new QHBoxLayout;
    QLabel *label_1 = new QLabel(this);
    label_1->setText("Old PDDB");
    label_1->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QLabel *label_2 = new QLabel(this);
    label_2->setText("New PDDB");
    label_2->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    centralLayout->addWidget(label_1);
    centralLayout->addWidget(upperListView);
    centralLayout->addWidget(label_2);
    centralLayout->addWidget(bottomListView);
    centralLayout->addWidget(centralButtonSubGroup);

    centralGroup->setLayout(centralLayout);

    clearListsBut = new QPushButton(tr("Clean"));
    compareBut = new QPushButton(tr("Compare"));

    centralButtonLayout->addWidget(clearListsBut);
    centralButtonLayout->addWidget(compareBut);
    centralButtonSubGroup->setLayout(centralButtonLayout);

    connect(clearListsBut, SIGNAL(clicked(bool)), this, SLOT(clean()));
    connect(compareBut, SIGNAL(clicked(bool)), this, SLOT(compare()));
    setCentralWidget(centralGroup);

    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("FZMSE"));

    setUnifiedTitleAndToolBarOnMac(true);
}

void appGUI::loadPathTo(const QString &type)
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                "",
                tr("XML (*.xml)"));

    std::string stdFilePath = filePath.toStdString();

    if (type == "oldPDDB")
        oldPDDBPath = stdFilePath;
    else if (type == "newPDDB")
        newPDDBPath = stdFilePath;
    else if (type == "oldGMC")
        oldGMCPath = stdFilePath;
    else if (type == "newGMC")
        newGMCPath = stdFilePath;
}

void appGUI::save()
{

}

void appGUI::generateRaport()
{

}

void appGUI::help()
{

}

void appGUI::insertCompareResult(const QString &result)
{
    if (result.isEmpty())
        return;

}

void appGUI::clean()
{
    upperListView->clear();
    bottomListView->clear();
}

void appGUI::compare()
{
    if (!(oldPDDBPath.empty() || newPDDBPath.empty() || oldGMCPath.empty()))
    {
        shared_ptr<PDDBDocument> docFirst = make_shared<PDDBDocument>(oldPDDBPath);
        shared_ptr<PDDBDocument> docSecound = make_shared<PDDBDocument>(newPDDBPath);


        std::vector<PDDBManagedObjectCompareResult> differences = PDDBDocument::compareDocuments(docFirst.get(), docSecound.get());

    }
}

QList<QStandardItem *> appGUI::listResultType(const QString &name,
                                           const QString &added,
                                           const QString &removed,
                                           const QString &modified)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(name);
    rowItems << new QStandardItem(added);
    rowItems << new QStandardItem(removed);
    rowItems << new QStandardItem(modified);
    return rowItems;
}

QList<QStandardItem *> appGUI::listMOCTemplate(const QString &className)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(className);
    return rowItems;
}

QList<QStandardItem *> appGUI::subListResultTypes(const QString &name, const QString &count)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(name);
    rowItems << new QStandardItem(count);
    return rowItems;
}

QList<QString *> appGUI::listOfLabels(const int &n)
{
    QList<QString *> labels;
    for (int i = 0; i < n; i++)
        labels << new QString("");
    return labels;
}

void appGUI::createLoadPathActions()
{
    QSignalMapper * mapper = new QSignalMapper(this);

    openOldPDDBAct = new QAction(tr("Open old PDDB"), this);
    openOldPDDBAct->setStatusTip(tr("Open old PDDB file "));
    connect(openOldPDDBAct, SIGNAL(triggered(bool)), mapper, SLOT(map()));

    openNewPDDBAct = new QAction(tr("Open new PDDB"), this);
    openNewPDDBAct->setStatusTip(tr("Open new PDDB file "));
    connect(openNewPDDBAct, SIGNAL(triggered(bool)), mapper, SLOT(map()));

    openOldGMCAct = new QAction(tr("Open old GMC"), this);
    openOldGMCAct->setStatusTip(tr("Open old GMC file "));
    connect(openOldGMCAct, SIGNAL(triggered(bool)), mapper, SLOT(map()));

    openNewGMCAct = new QAction(tr("Open new GMC"), this);
    openNewGMCAct->setStatusTip(tr("Open new GMC file "));
    connect(openNewGMCAct, SIGNAL(triggered(bool)), mapper, SLOT(map()));

    mapper->setMapping(openOldPDDBAct, "oldPDDB");
    mapper->setMapping(openNewPDDBAct, "newPDDB");
    mapper->setMapping(openOldGMCAct,  "oldGMC");
    mapper->setMapping(openNewGMCAct,  "newGMC");

    connect (mapper, SIGNAL(mapped(QString)), this, SLOT(loadPathTo(QString)));
}

void appGUI::createActions()
{
    createLoadPathActions();

    saveFileAct = new QAction(tr("Save file"), this);
    saveFileAct->setShortcut(QKeySequence::Save);
    saveFileAct->setStatusTip(tr("Save current file"));
    connect(saveFileAct, SIGNAL(triggered(bool)), this, SLOT(save()));

    generateRaportAct = new QAction(tr("&Generate raport"), this);
    generateRaportAct->setStatusTip(tr("Generate changes file"));
    connect(generateRaportAct, SIGNAL(triggered(bool)), this, SLOT(generateRaport()));

    displayHelpAct = new QAction(tr("&Help"), this);
    displayHelpAct->setStatusTip(tr("Help"));
    connect(displayHelpAct, SIGNAL(triggered(bool)), this, SLOT(help()));
}

void appGUI::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openOldPDDBAct);
    fileMenu->addAction(openNewPDDBAct);
    fileMenu->addAction(openOldGMCAct);
    fileMenu->addAction(openNewGMCAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addSeparator();

    raportMenu = menuBar()->addMenu(tr("&Raport"));
    raportMenu->addAction(generateRaportAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(displayHelpAct);
}

void appGUI::createToolBar()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(openOldPDDBAct);
    fileToolBar->addAction(openNewPDDBAct);
    fileToolBar->addAction(openOldGMCAct);
    fileToolBar->addAction(openNewGMCAct);
    fileToolBar->addAction(saveFileAct);

    raportToolBar = addToolBar(tr("&Raport"));
    raportToolBar->addAction(generateRaportAct);

}

void appGUI::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void appGUI::createDockWindows()
{
    dock = new QDockWidget(tr("Compare Result"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    resultTreeView = new QTreeView(dock);

    resultModel = new resultItemModel();
    resultModel->setRoot();
//    QStringList headers;
//    headers << "Name";
//    headers << "Count";
//    headers << "";
//    headers << "";
//    //resultList->setHorizontalHeaderLabels(headers);
//    //QStandardItem * item = resultList->invisibleRootItem();

//    QList<QStandardItem *> mockList = listResultType("Managed Object Class : ", "" , "", "");

//    QList<QStandardItem *> addedList = subListResultTypes("Added", " ");
//    QList<QStandardItem *> removedList = subListResultTypes("Removed", " ");
//    QList<QStandardItem *> modifiedList = subListResultTypes("Modified", " ");

//    //item->appendRow(mockList);

//    mockList.first()->appendRow(addedList);
//    mockList.first()->appendRow(removedList);
//    mockList.first()->appendRow(modifiedList);

//    resultTreeView->setModel(resultModel);
//    resultTreeView->expandAll();

    dock->setWidget(resultTreeView);

    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

}

std::vector<QString> appGUI::parseXmlByEndLine(std::string XML)
{
    std::string delimiter = "\n";
    std::vector<QString> result;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    while ((pos = XML.find(delimiter, prev)) != std::string::npos)
    {
        result.push_back(QString::fromStdString(XML.substr(prev, pos - prev)));
        prev = pos + 1;
    }
    result.push_back(QString::fromStdString(XML.substr(prev)));

    return result;
}
