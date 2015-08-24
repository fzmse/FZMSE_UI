#include <QtWidgets>
#include "Gui/appGui.h"
#include <cstring>
#include <QTextCursor>
#include <QTextCursor>
using namespace InternalTypes;

appGUI::appGUI()
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
    createPDDBResultDock();
    createPDDBResultView();
    createGMCResultDock();
    createGMCResultView();
    createPDDBTextDock();
    createGMCTextDock();

    setWindowTitle(tr("FZMSE"));

    setUnifiedTitleAndToolBarOnMac(true);
}

void appGUI::loadPathToDoc(const QString &type)
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                "",
                tr("XML (*.xml)"));

    std::string stdFilePath = filePath.toStdString();
    if (type == "oldPDDB")
    {
        oldPDDBPath = stdFilePath;
        oldPDDBdoc = make_shared<PDDBDocument>(oldPDDBPath);
        setLabel(oldPDDBLabel, "Old PDDB   ||   "+getFileName(oldPDDBPath));
        comparePDDB();

    }
    else if (type == "newPDDB")
    {
        newPDDBPath = stdFilePath;
        newPDDBdoc = make_shared<PDDBDocument>(newPDDBPath);
        setLabel(newPDDBLabel, "New PDDB   ||   "+getFileName(newPDDBPath));
        comparePDDB();

    }
    else if (type == "oldGMC")
    {
        oldGMCPath = stdFilePath;
        oldGMCdoc = make_shared<GMCDocument>(oldGMCPath);
        setLabel(oldGMCLabel, "Old GMC   ||   "+getFileName(oldGMCPath));
        compare();

    }
    else if (type == "newGMC")
    {
        newGMCPath = stdFilePath;
        newGMCdoc = make_shared<GMCDocument>(oldGMCPath);
        setLabel(newGMCLabel, "New GMC   ||   "+getFileName(newGMCPath));
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


void appGUI::clean()
{
    oldPDDBTextEdit->clear();
    newPDDBTextEdit->clear();
}

void appGUI::compare()
{
    if (!(oldPDDBPath.empty() || newPDDBPath.empty() || oldGMCPath.empty()))
    {
        statusBar()->showMessage(tr("Loading GMC actions"));
        vector<GMCAction> actions = GMCDocument::resolveGMCActions(oldPDDBdoc.get(), newPDDBdoc.get(), oldGMCdoc.get(), &differences);
        GMCResultModel->setResultVector(actions);
        GMCResultModel->setRoot();

        GMCResultView->setModel(GMCResultModel);

        connect(GMCResultView->selectionModel(),
                SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this,
                SLOT(showSelectedGMCResult()));
        GMCResultView->show();
        statusBar()->showMessage(tr("Ready"));
    }
}

void appGUI::comparePDDB()
{
    if (!(oldPDDBPath.empty() || newPDDBPath.empty()))
    {
        statusBar()->showMessage(tr("Loading PDDB's differences"));

        differences = PDDBDocument::compareDocuments(oldPDDBdoc.get(), newPDDBdoc.get());

        PDDBResultModel->setResultVector(differences);
        PDDBResultModel->setRoot();

        PDDBResultView->setModel(PDDBResultModel);

        connect(PDDBResultView->selectionModel(),
                SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this,
                SLOT(showSelectedPDDBResult()));

        PDDBResultView->show();
        statusBar()->showMessage(tr("Ready"));

    }
}

void appGUI::printDiff(resultItem *r)
{
    if (r != NULL)
    {
        auto origin = r->resultObj.getOrigin();
        auto scope = r->resultObj.getScope();
        clean();
        setLabels(r->getLocation(), false);

        if(origin == PDDBManagedObjectCompareResult::Added)
        {
            if ( scope == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
            {
                std::string s_xml = XmlElementReader::getXML(r->resultObj.getSecondElementComplexTypeParameter()->getElement());
                newPDDBTextEdit->setPlainText(QString::fromStdString(s_xml));
            }
            else
            {
                std::string s_xml = XmlElementReader::getXML(r->resultObj.getSecondElement()->getElement());
                newPDDBTextEdit->setPlainText(QString::fromStdString(s_xml));
            }
        }
        else if(origin == PDDBManagedObjectCompareResult::Removed)
        {
            if ( scope == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
            {
                std::string f_xml = XmlElementReader::getXML(r->resultObj.getFirstElementComplexTypeParameter()->getElement());
                oldPDDBTextEdit->setPlainText(QString::fromStdString(f_xml));
            }
            else
            {
                std::string f_xml = XmlElementReader::getXML(r->resultObj.getFirstElement()->getElement());
                oldPDDBTextEdit->setPlainText(QString::fromStdString(f_xml));
            }
        }
        else if(origin == PDDBManagedObjectCompareResult::Modified)
        {
            if ( scope == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
            {
                std::string f_xml = XmlElementReader::getXML(r->resultObj.getFirstElementComplexTypeParameter()->getElement());
                std::string s_xml = XmlElementReader::getXML(r->resultObj.getSecondElementComplexTypeParameter()->getElement());
                newPDDBTextEdit->setPlainText(QString::fromStdString(s_xml));
                oldPDDBTextEdit->setPlainText(QString::fromStdString(f_xml));
            }
            else
            {
                std::string f_xml = XmlElementReader::getXML(r->resultObj.getFirstElement()->getElement());
                std::string s_xml = XmlElementReader::getXML(r->resultObj.getSecondElement()->getElement());
                newPDDBTextEdit->setPlainText(QString::fromStdString(s_xml));
                oldPDDBTextEdit->setPlainText(QString::fromStdString(f_xml));
            }
        }
    }
    colorTextDifferences();
    statusBar()->showMessage(tr("Ready"));
}



void appGUI::setLabels(QString desc, bool PDDB)
{
    if ( PDDB )
    {
        oldPDDBLabel->setText(QString::fromStdString("Old PDDB   ||   "+getFileName(oldPDDBPath)+"   ||   ").append(desc));
        newPDDBLabel->setText(QString::fromStdString("New PDDB   ||   "+getFileName(newPDDBPath)+"   ||   ").append(desc));
    }
    else
    {
        oldGMCLabel->setText(QString::fromStdString("Old GMC   ||   "+getFileName(oldGMCPath)+"   ||   ").append(desc));
        newGMCLabel->setText(QString::fromStdString("New GMC   ||   "+getFileName(newGMCPath)+"   ||   ").append(desc));
    }
}

void appGUI::setLabel(QLabel * label, std::string text)
{
    label->setText(QString::fromStdString(text));
}

void appGUI::showSelectedPDDBResult()
{

    statusBar()->showMessage(tr("Filling boxes"));
    QModelIndex index = PDDBResultView->currentIndex();
    int currIntexRow = index.row();
    resultItem * r = PDDBResultModel->getItemFromRow(currIntexRow);
    GMCResultView->clearSelection();
    printDiff(r);
}

void appGUI::showSelectedGMCResult()
{
    statusBar()->showMessage(tr("Filling boxes"));
    QModelIndex index = GMCResultView->currentIndex();

    int currIntexRow = index.row();

    QItemSelection selection = QItemSelection();
    PDDBResultView->clearSelection();

    if ( index.parent().row() >= 0 )
    {
        gmcResultItem * gmcParentItem = GMCResultModel->getItemFromRow(index.parent().row());
        gmcResultItem * gmcChild = gmcParentItem->item(currIntexRow);
        string text = XmlElementReader::getXML(gmcChild->resultObj.getItem()->getElement());
        clean();
        newPDDBTextEdit->setPlainText(QString::fromStdString(text));
        setLabels(gmcChild->data(3).toString(), true);

    }
    else
    {
        gmcResultItem * gmcItem = GMCResultModel->getItemFromRow(currIntexRow);
        int gmcID = gmcItem->resultObj.getPDDBCompareResultId();
        resultItem * r = PDDBResultModel->getRoot()->findItemById(gmcID);
        printDiff(r);

    }
}

void appGUI::createPDDBResultView()
{
    PDDBResultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    PDDBResultView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void appGUI::createGMCResultView()
{
    GMCResultView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void appGUI::createPDDBTextDock()
{
    QHBoxLayout * texts;
    QHBoxLayout * labels;

    centralWid = new QWidget;

    verCentralLayout = new QVBoxLayout;

    centralWid->setLayout(verCentralLayout);

    setCentralWidget(centralWid);

    oldPDDBTextEdit = new QTextEdit();
    newPDDBTextEdit = new QTextEdit();
    oldPDDBTextEdit->setReadOnly(true);
    newPDDBTextEdit->setReadOnly(true);

    xmlHighlighterOldPDDB = make_shared<XMLHighlighter>(oldPDDBTextEdit ->document());
    xmlHighlighterNewPDDB = make_shared<XMLHighlighter>(newPDDBTextEdit->document());

    connect(oldPDDBTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            newPDDBTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    connect(newPDDBTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            oldPDDBTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    texts = new QHBoxLayout();

    texts->addWidget(oldPDDBTextEdit);
    texts->addWidget(newPDDBTextEdit);

    labels = new QHBoxLayout;

    oldPDDBLabel = new QLabel(this);
    oldPDDBLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    oldPDDBLabel->setText(tr("Old PDDB"));

    labels->addWidget(oldPDDBLabel, Qt::AlignLeft);


    newPDDBLabel = new QLabel(this);
    newPDDBLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    newPDDBLabel->setText(tr("New PDDB"));
    labels->addWidget(newPDDBLabel, Qt::AlignRight);

    verCentralLayout->addLayout(labels);
    verCentralLayout->addLayout(texts);

}

void appGUI::createGMCTextDock()
{
    QHBoxLayout * texts;
    QHBoxLayout * labels;

    oldGMCTextEdit = new QTextEdit();
    newGMCTextEdit = new QTextEdit();
    oldGMCTextEdit->setReadOnly(true);
    newGMCTextEdit->setReadOnly(true);

    xmlHighlighterOldGMC = make_shared<XMLHighlighter>(oldGMCTextEdit ->document());
    xmlHighlighterNewGMC = make_shared<XMLHighlighter>(newGMCTextEdit->document());

    oldGMCTextEdit->setMaximumHeight(this->height() * 0.2);
    newGMCTextEdit->setMaximumHeight(this->height() * 0.2);

    connect(oldGMCTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            newGMCTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    connect(newGMCTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            oldGMCTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    texts = new QHBoxLayout();

    texts->addWidget(oldGMCTextEdit);
    texts->addWidget(newGMCTextEdit);

    labels = new QHBoxLayout;

    oldGMCLabel = new QLabel(this);
    oldGMCLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    oldGMCLabel->setText(tr("Old GMC"));
    labels->addWidget(oldGMCLabel, Qt::AlignLeft);


    newGMCLabel = new QLabel(this);
    newGMCLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    labels->addWidget(newGMCLabel, Qt::AlignRight);
    newGMCLabel->setText(tr("New GMC"));
    verCentralLayout->addLayout(labels);
    verCentralLayout->addLayout(texts);

}

std::string appGUI::getFileName(string path)
{
    size_t found = path.find_last_of("/\\");
    return  path.substr(found+1);
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

    connect (mapper, SIGNAL(mapped(QString)), this, SLOT(loadPathToDoc(QString)));
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

void appGUI::createPDDBResultDock()
{
    PDDBResultDock = new QDockWidget(tr("PDDB differences"), this);
    PDDBResultDock->setAllowedAreas(Qt::RightDockWidgetArea);
    PDDBResultView = new QTreeView(PDDBResultDock);

    PDDBResultView->setStyleSheet("QTreeView::item:selected{background-color: rgb(102,255,102);}");
    PDDBResultModel = new resultItemModel();

    PDDBResultDock->setWidget(PDDBResultView);
    addDockWidget(Qt::RightDockWidgetArea, PDDBResultDock);
    viewMenu->addAction(PDDBResultDock->toggleViewAction());

}

void appGUI::createGMCResultDock()
{
    GMCResultDock = new QDockWidget(tr("GMC Actions"), this);
    GMCResultDock->setAllowedAreas(Qt::RightDockWidgetArea);
    GMCResultView = new QTreeView(GMCResultDock);
    GMCResultView->setStyleSheet("QTreeView::item:selected{background-color: rgb(102,255,102);}");
    GMCResultModel = new gmcResultItemModel();

    GMCResultDock->setWidget(GMCResultView);

    addDockWidget(Qt::RightDockWidgetArea, GMCResultDock);
    viewMenu->addAction(GMCResultDock->toggleViewAction());

}

void appGUI::createPDDBDescriptionDock()
{
    PDDBDescriptionDock = new QDockWidget(tr("Description"), this);
    PDDBDescriptionDock->setAllowedAreas(Qt::TopDockWidgetArea);
    PDDBDescriptionDock->setWidget(new QTextEdit );

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

inline moveToLine(QTextCursor &cur, int line)
{
    cur.movePosition(QTextCursor::Start);
    cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);
}

inline incrementVectorFromIntex(vector<pair<int, int> > &para, int index, bool first, int incrementValue)
{
    for ( int i = index; i < para.size(); i++ )
    {
        if ( first )
        {
            (&para[i])->first += incrementValue;
        }
        else
        {
            (&para[i])->second += incrementValue;
        }
    }
}

void appGUI::colorTextDifferences()
{
    QTextDocument * up_doc = oldPDDBTextEdit->document();
    QTextDocument * bt_doc = newPDDBTextEdit->document();

    QTextCursor up_cur(up_doc);
    QTextCursor bt_cur(bt_doc);

    QTextBlockFormat dark_pink = up_cur.blockFormat();
    QTextBlockFormat grey = up_cur.blockFormat();
    QTextBlockFormat white = up_cur.blockFormat();

    dark_pink.setBackground(QBrush(QColor(120, 0, 97, 60)));
    grey.setBackground(QBrush(QColor(60, 60, 60, 30)));

    //qDebug()<< "First doc lines: " << up_doc->lineCount() << "\n Second doc lines: " << bt_doc->lineCount();

    std::vector<pair<int, int>> repetedLines;

    int first_doc_pos = 0;
    int second_doc_pos = 0;

    for ( int i = first_doc_pos; i < up_doc->lineCount(); i++ )
    {
        moveToLine(up_cur, i);
        for ( int j = second_doc_pos; j < bt_doc->lineCount(); j++ )
        {
            moveToLine(bt_cur, j);
            if ( up_cur.block().text() ==  bt_cur.block().text() )
            {
                repetedLines.push_back(std::pair<int, int>(i, j));
                second_doc_pos = j + 1;
                break;
            }
        }
    }

    if ( ! repetedLines.empty() )
    {
        for ( int i = 0; i < repetedLines.size()-1; ++i )
        {
            pair<int, int> fP = repetedLines[i];
            pair<int, int> sP = repetedLines[i+1];

            int fdiff = sP.first - fP.first;
            int blockdiff = sP.first - sP.second;


            for ( int j = 1; j < fdiff; j++ )
            {
                moveToLine(up_cur, fP.first + j);

                up_cur.setBlockFormat(dark_pink);
            }

            int sdiff = sP.second - fP.second;
            for ( int j = 1; j < sdiff; j++ )
            {
                moveToLine(bt_cur, fP.second + j);
                bt_cur.setBlockFormat(dark_pink);
            }

        }
        for ( int i = 0; i < repetedLines.size(); ++i )
        {
            pair<int, int> para = repetedLines[i];
            int diff = para.first - para.second;
            if ( diff < 0 )
            {
                int inc = abs(diff);
                moveToLine(up_cur, para.first);
                up_cur.setBlockFormat(grey);
                for ( int x = 0; x < inc; x++)
                {
                    up_cur.insertBlock();

                }
                up_cur.setBlockFormat(white);
                incrementVectorFromIntex(repetedLines, i, true, inc);
            }
            else
            {
                int inc = abs(diff);
                moveToLine(bt_cur, para.second);
                bt_cur.setBlockFormat(grey);
                for ( int x = 0; x < inc; x++)
                {
                    bt_cur.insertBlock();

                }
                bt_cur.setBlockFormat(white);
                incrementVectorFromIntex(repetedLines, i, false, inc);
            }
        }
    }
}
