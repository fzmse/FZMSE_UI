#include "Gui/appGui.h"

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

    setWindowTitle(tr("GMC Automation Tool"));

    setUnifiedTitleAndToolBarOnMac(true);

    setWindowIcon(QIcon(":report/icon_win.png"));
}

void appGUI::loadPathToDoc(const QString &type)
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                "",
                tr("XML (*.xml)"));
    if (filePath.length() > 0)
    {
        std::string stdFilePath = filePath.toStdString();
        if (type == "oldPDDB")
        {
            clean();
            oldPDDBPath = stdFilePath;
            oldPDDBdoc = make_shared<PDDBDocument>(oldPDDBPath);
            setLabel(oldPDDBLabel, "Old PDDB   ||   "+getFileName(oldPDDBPath));
            comparePDDB();
            compare();
        }
        else if (type == "newPDDB")
        {
            clean();
            newPDDBPath = stdFilePath;
            newPDDBdoc = make_shared<PDDBDocument>(newPDDBPath);
            setLabel(newPDDBLabel, "New PDDB   ||   "+getFileName(newPDDBPath));
            comparePDDB();
            compare();
        }
        else if (type == "oldGMC")
        {
            oldGMCPath = stdFilePath;
            oldGMCdoc = make_shared<GMCDocument>(oldGMCPath);
            setLabel(oldGMCLabel, "Old GMC   ||   "+getFileName(oldGMCPath));
            compare();
        }
    }
    else
    {

    }
}

void appGUI::setCurrDist(QModelIndex index)
{
    auto var = distListModel->data(index, Qt::DisplayRole);
    currDist = var.toString();
}

void appGUI::setDistToAction()
{
    string var = currDist.toStdString();
    currAction->buildDistNameFromBase(var);
    newGMCdoc = make_shared<GMCDocument>(oldGMCdoc.get());
    GMCWriter::reactToAll(newGMCdoc.get(), actions);
    dialogList->close();
    showSelectedGMCResult();
}

void appGUI::closeDistNameDialog()
{
    dialogList->close();
}

void appGUI::save()
{
    if (!(oldPDDBPath.empty() || newPDDBPath.empty() || oldGMCPath.empty()))
    {
        QString savePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save file"),
                    tr(oldGMCPath.c_str()),
                    tr("XML (*.xml)"));
        if (savePath.length() > 0)
        {
            newGMCdoc = make_shared<GMCDocument>(oldGMCdoc.get());
            auto report = GMCWriter::reactToAllIncluded(newGMCdoc.get(), actions);
            GMCWriter::updateVersionInGmc(newGMCdoc.get(), newPDDBdoc->getReleaseVersion(), newPDDBdoc->getReleaseName());
            if (XmlWriter::save(newGMCdoc.get()->getXMLDocument(), savePath.toStdString()))
            {
                QDir d = QFileInfo(savePath).absoluteDir();
                QString fName = QFileInfo(savePath).fileName();
                QString logoPath = d.absolutePath() + "/logo.png";
                QString reportPath = savePath;
                reportPath += ".html";
                ReportUtilities::saveLogo(logoPath.toStdString());
                ReportUtilities::generateReport(report, reportPath.toStdString(),
                                                getFileName(oldPDDBPath), getFileName(newPDDBPath),
                                                getFileName(oldGMCPath), fName.toStdString(), newPDDBdoc->getReleaseName());
                QMessageBox::information(this, tr("Saving file"),
                                               tr("Success !         "),
                                               QMessageBox::Ok);
            }
            else
            {
                QMessageBox::warning(this, tr("Saving file"),
                                               tr("Failed!          "),
                                               QMessageBox::Ok);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Saving file"),
                                       tr("PDDB or GMC files not loaded"),
                                       QMessageBox::Ok);
    }
}

void appGUI::help()
{
    helpDialog * dialog = new helpDialog();


}


void appGUI::clean()
{
    oldPDDBTextEdit->clear();
    newPDDBTextEdit->clear();
    oldGMCTextEdit->clear();
    newGMCTextEdit->clear();


    setLabels("", true);
    if ( !oldGMCPath.empty() )
        setLabels("", false);
}

void appGUI::onGMCRClick(const QPoint &pos)
{
    QPoint globalPos = GMCResultView->mapToGlobal(pos);
    auto index = GMCResultView->indexAt(pos);
    auto item = GMCResultModel->getItemFromRow(index.row());
    if ( index.parent().row() == -1 && item->resultObj.getChangeScope() == GMCAction::ManagedObject)
    {
        QMenu myMenu;
        myMenu.addAction(setDistName);
        myMenu.addAction(addToGMC);
        myMenu.addAction(delFromGMC);

        QAction* selectedItem = myMenu.exec(globalPos);

        int id = item->resultObj.getPDDBCompareResultId();

        for (int i = 0; i < actions.size(); i++)
        {
            if ( actions[i].getPDDBCompareResultId() == id )
            {
                if (addToGMC == selectedItem)
                {
                    actions[i].setIncludedInGMC(true);
                    item->resultObj.setIncludedInGMC(true);
                    item->updateIncludedInGMC(true);
                    break;
                } else if ( delFromGMC == selectedItem )
                {
                    actions[i].setIncludedInGMC(false);
                    item->resultObj.setIncludedInGMC(false);
                    item->updateIncludedInGMC(false);
                    break;
                } else if ( setDistName == selectedItem )
                {
                    currAction = &actions[i];
                    choiceDistName();
                }
            }
        }
    }
    else if ( index.parent().row() == -1)
    {
        QMenu myMenu;
        myMenu.addAction(addToGMC);
        myMenu.addAction(delFromGMC);

        QAction* selectedItem = myMenu.exec(globalPos);

        int id = item->resultObj.getPDDBCompareResultId();

        for (int i = 0; i < actions.size(); i++)
        {
            if ( actions[i].getPDDBCompareResultId() == id )
            {
                if (addToGMC == selectedItem)
                {
                    actions[i].setIncludedInGMC(true);
                    item->resultObj.setIncludedInGMC(true);
                    item->updateIncludedInGMC(true);
                    break;
                } else if ( delFromGMC == selectedItem )
                {
                    actions[i].setIncludedInGMC(false);
                    item->resultObj.setIncludedInGMC(false);
                    item->updateIncludedInGMC(false);
                    break;
                }
            }
        }
    }
}

void appGUI::setPDDBHint(QModelIndex index)
{
    PDDBResultView->setToolTip(PDDBResultModel->data(index, Qt::DisplayRole).toString());
}

void appGUI::setGMCHint(QModelIndex index)
{
    GMCResultView->setToolTip(GMCResultModel->data(index, Qt::DisplayRole).toString());
}

void appGUI::choiceDistName()
{
    dialogList = new QDialog();
    dialogList->setWindowFlags(Qt::WindowStaysOnTopHint);
    dialogList->setModal(true);
    dialogList->setWindowTitle(tr("GMC Action"));
    QVBoxLayout * dialogLayout = new QVBoxLayout();
    QStringList distNameList;

    vector<string> distV = oldGMCdoc->getDistNames();

    for (vector<string>::iterator it = distV.begin(); it != distV.end(); it++)
    {
        distNameList << QString::fromStdString(*it);
    }

    distListModel = new QStringListModel(this);

    distListModel->setStringList(distNameList);

    QListView * distListView = new QListView(this);

    distListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    distListView->setModel(distListModel);

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    QPushButton * select = new QPushButton(tr("Select"));
    QPushButton * cancel = new QPushButton(tr("Cancel"));
    buttonsLayout->addWidget(select);
    buttonsLayout->addWidget(cancel);

    connect(distListView, SIGNAL(pressed(QModelIndex)), this, SLOT(setCurrDist(QModelIndex)));

    connect(select, SIGNAL(clicked(bool)), this, SLOT(setDistToAction()));

    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(closeDistNameDialog()));

    QHBoxLayout * listLayout = new QHBoxLayout();
    listLayout->addWidget(distListView);

    dialogLayout->addLayout(listLayout);
    dialogLayout->addLayout(buttonsLayout);

    dialogList->setLayout(dialogLayout);
    dialogList->show();
}


void appGUI::compare()
{
    if (!(oldPDDBPath.empty() || newPDDBPath.empty() || oldGMCPath.empty()))
    {

        statusBar()->showMessage(tr("Loading GMC actions"));
        actions = GMCDocument::resolveGMCActions(oldPDDBdoc.get(), newPDDBdoc.get(), oldGMCdoc.get(), &differences);

        if ( actions.size() == 0 )
            QMessageBox::information(this, tr("GMC"),
                                           tr("No actions available !     "),
                                           QMessageBox::Ok);

        GMCResultModel->setResultVector(actions);
        GMCResultModel->setRoot();
        GMCResultView->reset();
        GMCResultView->setModel(GMCResultModel);
        QHeaderView *headerView = new QHeaderView(Qt::Horizontal, this);
        GMCResultView->setHeader(headerView);

        GMCResultView->setColumnWidth(0, 40);
        GMCResultView->setColumnWidth(1, 20);
        GMCResultView->setColumnWidth(2, 75);
        GMCResultView->setColumnWidth(3, 20);
        headerView->setSectionResizeMode(4, QHeaderView::Stretch);
        headerView->setSectionResizeMode(5, QHeaderView::Stretch);

        headerView->setHidden(false);

        GMCResultView->setMouseTracking(true);

        connect(GMCResultView,
                SIGNAL(clicked(QModelIndex)),
                this,
                SLOT(showSelectedGMCResult()));

        connect(GMCResultView,
                SIGNAL(customContextMenuRequested(const QPoint &)),
                this,
                SLOT(onGMCRClick(const QPoint &)));

        connect(GMCResultView,
                SIGNAL(entered(QModelIndex)),
                this,
                SLOT(setGMCHint(QModelIndex)));

        GMCResultView->show();
        statusBar()->showMessage(tr("Ready"));

        newGMCdoc = make_shared<GMCDocument>(oldGMCdoc.get());
        GMCWriter::reactToAll(newGMCdoc.get(), actions);
    }
}

void appGUI::comparePDDB()
{
    if (!(oldPDDBPath.empty() || newPDDBPath.empty()))
    {
        statusBar()->showMessage(tr("Loading PDDB's differences"));

        differences = PDDBDocument::compareDocuments(oldPDDBdoc.get(), newPDDBdoc.get());

        if ( differences.size() == 0 )
            QMessageBox::information(this, tr("PDDB"),
                                           tr("No differences found !     "),
                                           QMessageBox::Ok);
        PDDBResultModel->setResultVector(differences);
        PDDBResultModel->setRoot();
        PDDBResultView->reset();

        PDDBResultView->setModel(PDDBResultModel);

        QHeaderView *headerView = new QHeaderView(Qt::Horizontal, this);
        PDDBResultView->setHeader(headerView);

        headerView->setSectionResizeMode(2, QHeaderView::Stretch);
        headerView->setSectionResizeMode(3, QHeaderView::Stretch);

        PDDBResultView->setColumnWidth(0, 80);
        PDDBResultView->setColumnWidth(1, 20);
        headerView->setHidden(false);
        PDDBResultView->setMouseTracking(true);

        connect(PDDBResultView,
                SIGNAL(clicked(QModelIndex)),
                this,
                SLOT(showSelectedPDDBResult()));

        connect(PDDBResultView,
                SIGNAL(entered(QModelIndex)),
                this,
                SLOT(setPDDBHint(QModelIndex)));

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
        newGMCLabel->setText(QString::fromStdString("New GMC   ||   ").append(desc));
    }
}

void appGUI::setLabel(QLabel * label, std::string text)
{
    label->setText(QString::fromStdString(text));
}

void appGUI::changeUserInteraction(gmcResultItem *item)
{
    int id = item->resultObj.getPDDBCompareResultId();
    for (int i = 0; i < actions.size(); i++)
    {
        if ( actions[i].getPDDBCompareResultId() == id )
        {
            if ( !item->resultObj.isIncludedInGMC() )
            {
                actions[i].setIncludedInGMC(true);
                item->resultObj.setIncludedInGMC(true);
                item->updateIncludedInGMC(true);
                break;
            } else
            {
                actions[i].setIncludedInGMC(false);
                item->resultObj.setIncludedInGMC(false);
                item->updateIncludedInGMC(false);
                break;
            }
        }
    }
}

void appGUI::showSelectedPDDBResult()
{

    statusBar()->showMessage(tr("Filling boxes"));
    QModelIndex index = PDDBResultView->currentIndex();
    int currIntexRow = index.row();
    resultItem * r = PDDBResultModel->getItemFromRow(currIntexRow);

    GMCResultView->clearSelection();
    auto pddbIndex = PDDBResultView->currentIndex();
    PDDBResultView->selectionModel()->select(PDDBResultModel->index(pddbIndex.row(), pddbIndex.column()),
                                             QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

    printDiff(r);

    setLabels(r->data(2).toString(), true);
    setLabels("", false);
    for ( int i = 0; i < actions.size(); i++)
    {
        if ( actions[i].getPDDBCompareResultId() == r->resultObj.getId() )
        {
            printDiff(r);
            setLabels(r->data(2).toString(), true);
            setLabels(r->data(2).toString(), false);
            pair<string, string> gmcPair = GMCDocument::resolveGMCCompareText(oldGMCdoc.get(), newGMCdoc.get(), actions[i]);
            oldGMCTextEdit->setPlainText(QString::fromStdString(gmcPair.first));
            newGMCTextEdit->setPlainText(QString::fromStdString(gmcPair.second));

            GMCResultView->selectionModel()->select(GMCResultModel->index(i,0),
                                                     QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            GMCResultView->scrollTo(GMCResultModel->index(i,0));
            break;
        }
    }
    statusBar()->showMessage(tr("Ready"));
}

void appGUI::showSelectedGMCResult()
{
    statusBar()->showMessage(tr("Filling boxes"));
    QModelIndex index = GMCResultView->currentIndex();

    int currIntexRow = index.row();
    QItemSelection selection = QItemSelection();

    if ( index.parent().row() >= 0 )
    {
        auto gmcIndex = GMCResultView->currentIndex();

        GMCResultView->selectionModel()->select(GMCResultModel->index(index.parent().row(), gmcIndex.column()),
                                                 QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        gmcResultItem * gmcParentItem = GMCResultModel->getItemFromRow(index.parent().row());
        gmcResultItem * gmcChild = gmcParentItem->item(currIntexRow);
        string text = XmlElementReader::getXML(gmcChild->resultObj.getItem()->getElement());
        clean();
        newPDDBTextEdit->setPlainText(QString::fromStdString(text));
        setLabels(gmcChild->data(2).toString(), true);
        setLabels(gmcChild->data(2).toString(), false);
        pair<string, string> gmcPair = GMCDocument::resolveGMCCompareText(oldGMCdoc.get(), newGMCdoc.get(), gmcChild->resultObj);
        oldGMCTextEdit->setPlainText(QString::fromStdString(gmcPair.first));
        newGMCTextEdit->setPlainText(QString::fromStdString(gmcPair.second));
    }
    else
    {
        auto gmcIndex = GMCResultView->currentIndex();

        GMCResultView->selectionModel()->select(GMCResultModel->index(gmcIndex.row(), gmcIndex.column()),
                                                 QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        gmcResultItem * gmcItem = GMCResultModel->getItemFromRow(currIntexRow);
        int gmcID = gmcItem->resultObj.getPDDBCompareResultId();
        resultItem * r = PDDBResultModel->getRoot()->findItemById(gmcID);

        for ( int i = 0; i < differences.size(); i++)
        {
            if ( differences[i].getId() == gmcID )
            {
                PDDBResultView->selectionModel()->select(PDDBResultModel->index(i,0),
                                                         QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                PDDBResultView->scrollTo(PDDBResultModel->index(i,0));
                break;
            }
        }

        printDiff(r);
        setLabels(r->data(2).toString(), true);
        setLabels(r->data(2).toString(), false);
        pair<string, string> gmcPair = GMCDocument::resolveGMCCompareText(oldGMCdoc.get(), newGMCdoc.get(), gmcItem->resultObj);
        oldGMCTextEdit->setPlainText(QString::fromStdString(gmcPair.first));
        newGMCTextEdit->setPlainText(QString::fromStdString(gmcPair.second));


    }
    statusBar()->showMessage(tr("Ready"));
}

void appGUI::createPDDBResultView()
{
    PDDBResultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    PDDBResultView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void appGUI::createGMCResultView()
{
    GMCResultView->setSelectionMode(QAbstractItemView::SingleSelection);
    GMCResultView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void appGUI::createPDDBTextDock()
{
    QHBoxLayout * texts;
    QHBoxLayout * labels;

    centralWid = new QWidget;

    centralWid->setMinimumWidth(this->width()* 0.4 );

    verCentralLayout = new QVBoxLayout;

    centralWid->setLayout(verCentralLayout);

    setCentralWidget(centralWid);

    oldPDDBTextEdit = new QTextEdit();
    newPDDBTextEdit = new QTextEdit();
    oldPDDBTextEdit->setReadOnly(true);
    newPDDBTextEdit->setReadOnly(true);
    oldPDDBTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    newPDDBTextEdit->setLineWrapMode(QTextEdit::NoWrap);


    xmlHighlighterOldPDDB = make_shared<XMLHighlighter>(oldPDDBTextEdit->document());
    xmlHighlighterNewPDDB = make_shared<XMLHighlighter>(newPDDBTextEdit->document());

    connect(oldPDDBTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            newPDDBTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    connect(newPDDBTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            oldPDDBTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    connect(oldPDDBTextEdit->horizontalScrollBar(),
            SIGNAL(valueChanged(int)),
            newPDDBTextEdit->horizontalScrollBar(),
            SLOT(setValue(int)));

    connect(newPDDBTextEdit->horizontalScrollBar(),
            SIGNAL(valueChanged(int)),
            oldPDDBTextEdit->horizontalScrollBar(),
            SLOT(setValue(int)));

    texts = new QHBoxLayout();

    texts->addWidget(oldPDDBTextEdit);
    texts->addWidget(newPDDBTextEdit);

    labels = new QHBoxLayout;

    oldPDDBLabel = new QLabel(this);
    oldPDDBLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    oldPDDBLabel->setWordWrap(true);
    oldPDDBLabel->setText(tr("Old PDDB"));

    labels->addWidget(oldPDDBLabel, Qt::AlignTop);


    newPDDBLabel = new QLabel(this);
    newPDDBLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    newPDDBLabel->setWordWrap(true);
    newPDDBLabel->setText(tr("New PDDB"));

    labels->addWidget(newPDDBLabel, Qt::AlignTop);

    verCentralLayout->addLayout(labels);
    verCentralLayout->addLayout(texts);

    QFont font = oldPDDBLabel->font();
    font.setBold(true);

    oldPDDBLabel->setFont(font);
    newPDDBLabel->setFont(font);

}

void appGUI::createGMCTextDock()
{
    QHBoxLayout * texts;
    QHBoxLayout * labels;

    oldGMCTextEdit = new QTextEdit();
    newGMCTextEdit = new QTextEdit();
    oldGMCTextEdit->setReadOnly(true);
    newGMCTextEdit->setReadOnly(true);
    oldGMCTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    newGMCTextEdit->setLineWrapMode(QTextEdit::NoWrap);


    xmlHighlighterOldGMC = make_shared<XMLHighlighter>(oldGMCTextEdit ->document());
    xmlHighlighterNewGMC = make_shared<XMLHighlighter>(newGMCTextEdit->document());

    oldGMCTextEdit->setMaximumHeight(this->height() * 0.2);
    oldGMCTextEdit->setMinimumWidth(400);
    newGMCTextEdit->setMaximumHeight(this->height() * 0.2);
    newGMCTextEdit->setMinimumWidth(400);

    connect(oldGMCTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            newGMCTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    connect(newGMCTextEdit->verticalScrollBar(),
            SIGNAL(valueChanged(int)),
            oldGMCTextEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    connect(oldGMCTextEdit->horizontalScrollBar(),
            SIGNAL(valueChanged(int)),
            newGMCTextEdit->horizontalScrollBar(),
            SLOT(setValue(int)));

    connect(newGMCTextEdit->horizontalScrollBar(),
            SIGNAL(valueChanged(int)),
            oldGMCTextEdit->horizontalScrollBar(),
            SLOT(setValue(int)));


    texts = new QHBoxLayout();

    texts->addWidget(oldGMCTextEdit);
    texts->addWidget(newGMCTextEdit);

    labels = new QHBoxLayout;

    oldGMCLabel = new QLabel(this);

    oldGMCLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    oldGMCLabel->setText(tr("Old GMC"));
    oldGMCLabel->setWordWrap(true);
    labels->addWidget(oldGMCLabel, Qt::AlignTop);


    newGMCLabel = new QLabel(this);
    newGMCLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    newGMCLabel->setWordWrap(true);
    newGMCLabel->setText(tr("New GMC"));
    labels->addWidget(newGMCLabel, Qt::AlignTop);

    verCentralLayout->addLayout(labels);
    verCentralLayout->addLayout(texts);

    QFont font = oldGMCLabel->font();
    font.setBold(true);

    oldGMCLabel->setFont(font);
    newGMCLabel->setFont(font);
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

    mapper->setMapping(openOldPDDBAct, "oldPDDB");
    mapper->setMapping(openNewPDDBAct, "newPDDB");
    mapper->setMapping(openOldGMCAct,  "oldGMC");

    connect (mapper, SIGNAL(mapped(QString)), this, SLOT(loadPathToDoc(QString)));
}

void appGUI::createActions()
{
    createLoadPathActions();

    saveFileAct = new QAction(tr("Save file"), this);
    saveFileAct->setShortcut(QKeySequence::Save);
    saveFileAct->setStatusTip(tr("Save current file"));
    connect(saveFileAct, SIGNAL(triggered(bool)), this, SLOT(save()));

    displayHelpAct = new QAction(tr("&Help"), this);
    displayHelpAct->setStatusTip(tr("Help"));
    connect(displayHelpAct, SIGNAL(triggered(bool)), this, SLOT(help()));

    addToGMC = new QAction(tr("Include in GMC"), this);
    //connect(addToGMC, SIGNAL(triggered(bool)), this, SLOT(includeInGMC(true)));

    delFromGMC = new QAction(tr("Exclude from GMC"), this);
    //connect(delToGMC, SIGNAL(triggered(bool)), this, SLOT(includeInGMC(false)));

    setDistName = new QAction(tr("Setup MOC distName in GMC"), this);
    //connect(setDistName, SIGNAL(triggered(bool)), this, SLOT(choiceDistName(GMCAction)));

    selectClicked = new QAction(tr("Confirm selection"), this);
}

void appGUI::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openOldPDDBAct);
    fileMenu->addAction(openNewPDDBAct);
    fileMenu->addAction(openOldGMCAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addSeparator();

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(displayHelpAct);
}

void appGUI::createToolBar()
{
    fileToolBar = new QToolBar(tr("&File"));
    addToolBar(Qt::LeftToolBarArea, fileToolBar);
    fileToolBar->setIconSize(QSize(48, 48));
    openOldPDDBAct->setIcon(QIcon(":/report/opic.png"));
    openNewPDDBAct->setIcon(QIcon(":/report/npic.png"));
    openOldGMCAct->setIcon(QIcon(":/report/ogic.png"));
    saveFileAct->setIcon(QIcon(":/report/sic.png"));
    fileToolBar->addAction(openOldPDDBAct);
    fileToolBar->addAction(openNewPDDBAct);
    fileToolBar->addAction(openOldGMCAct);
    fileToolBar->addAction(saveFileAct);

}

void appGUI::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void appGUI::createPDDBResultDock()
{
    PDDBResultDock = new QDockWidget(tr("PDDB differences"), this);
    PDDBResultDock->setAllowedAreas(Qt::RightDockWidgetArea);
    PDDBResultDock->setMinimumWidth(this->width() * 0.3);

    PDDBResultView = new QTreeView(PDDBResultDock);

    PDDBResultView->setStyleSheet("QTreeView::item:selected{"
                                  "background-color: rgb(102,255,102);"
                                  "color: black;"
                                  "}");


    PDDBResultDock->setStyleSheet("QDockWidget {"
                                 "font-size: 13px;"
                                 "font-weight: bold;"
                                 "}");



    PDDBResultModel = new resultItemModel();

    PDDBResultDock->setWidget(PDDBResultView);
    addDockWidget(Qt::RightDockWidgetArea, PDDBResultDock);
    viewMenu->addAction(PDDBResultDock->toggleViewAction());

}

void appGUI::createGMCResultDock()
{
    GMCResultDock = new QDockWidget(tr("GMC Actions"), this);
    GMCResultDock->setAllowedAreas(Qt::RightDockWidgetArea);
    GMCResultDock->setMinimumWidth(this->width() * 0.3);
    GMCResultView = new QTreeView(GMCResultDock);
    GMCResultView->setStyleSheet("QTreeView::item:selected {"
                                 "background-color: rgb(102,255,102);"
                                 "color: black;"
                                 "}");

    GMCResultDock->setStyleSheet("QDockWidget {"
                                 "font-size: 13px;"
                                 "font-weight: bold;"
                                 "}");


    GMCResultModel = new gmcResultItemModel(this);
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

vector<GMCAction> * appGUI::getActions()
{
    return &actions;
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
