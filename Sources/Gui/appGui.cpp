#include <QtWidgets>

#include "Gui/appGui.h"
#include <cstring>

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

void appGUI::openOldPDDB()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                "",
                tr("XML (*.xml)"));

    reader = new XmlReader(XmlWrapper::loadDocument(filePath.toStdString()));

    std::string xml = elementReader->getXML(reader->getCurrElement());

    QString qXml = QString::fromStdString(xml);

    upperListView->addItem(qXml);

//    QFile file(filePath);

//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//       return;

//    upperListView->clear();
//    QTextStream in(&file);
//    while (!in.atEnd())
//    {
       // upperListView->addItem(in.readLine());
//    }
}

void appGUI::openNewPDDB()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                "",
                tr("XML (*.xml)"));

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       return;

    bottomListView->clear();
    QTextStream in(&file);
    while (!in.atEnd())
    {
        bottomListView->addItem(in.readLine());
    }
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

}

void appGUI::createActions()
{
    openOldPDDBAct = new QAction(tr("Open old PDDB"), this);
    openOldPDDBAct->setStatusTip(tr("Open old PDDB file "));
    connect(openOldPDDBAct, SIGNAL(triggered(bool)), this, SLOT(openOldPDDB()));

    openNewPDDBAct = new QAction(tr("Open new PDDB"), this);
    openNewPDDBAct->setStatusTip(tr("Open new PDDB file "));
    connect(openNewPDDBAct, SIGNAL(triggered(bool)), this, SLOT(openNewPDDB()));

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
    QDockWidget *dock = new QDockWidget(tr("Compare Result"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    resultList = new QListWidget(dock);

    dock->setWidget(resultList);

    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());


    connect(resultList, SIGNAL(currentTextChanged(QString)),
            this, SLOT(insertCompareResult(QString)));

}
