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
    setConnections();

    setUnifiedTitleAndToolBarOnMac(true);

    setWindowTitle(tr("GMC Automation Tool"));
    setWindowIcon(QIcon(":report/icon_win.png"));

    dialogList = NULL;
    saveDialog = NULL;
    fixDialog = NULL;
    hzDialog = NULL;
    helpDialog = NULL;

    toBeSorted = false;
    templatePath = "";

    fixWithSort = false;
    fixFilePath = "";
}

void appGUI::loadPathToDoc(const QString &type)
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\GMCAutomationTool",
             QSettings::NativeFormat);

    QString diff_path(tr(""));
    QString openType(tr(""));

    if ( type == "oldPDDB" )
    {
        diff_path = settings.value("Old_PDDB_path").toString();
        openType.append("Old PDDB file");
    }
    else if (type == "newPDDB")
    {
       diff_path = settings.value("New_PDDB_path").toString();
       openType.append("New PDDB File");
    }
    else if ( type == "oldGMC" )
    {
        diff_path = settings.value("GMC_path").toString();
        openType.append("Old GMC File");
    }



    QString filePath = QFileDialog::getOpenFileName(
                this,
                openType,
                diff_path,
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
            settings.setValue("Old_PDDB_path", filePath);
            comparePDDB();
            compare();
        }
        else if (type == "newPDDB")
        {
            clean();
            newPDDBPath = stdFilePath;
            newPDDBdoc = make_shared<PDDBDocument>(newPDDBPath);
            setLabel(newPDDBLabel, "New PDDB   ||   "+getFileName(newPDDBPath));
            settings.setValue("New_PDDB_path", filePath);
            comparePDDB();
            compare();
        }
        else if (type == "oldGMC")
        {
            oldGMCPath = stdFilePath;
            oldGMCdoc = make_shared<GMCDocument>(oldGMCPath);
            setLabel(oldGMCLabel, "Old GMC   ||   "+getFileName(oldGMCPath));
            settings.setValue("GMC_path", filePath);
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
    if ( currAction != NULL)
    {
        string var = currDist.toStdString();
        currAction->buildDistNameFromBase(var);
        newGMCdoc = make_shared<GMCDocument>(oldGMCdoc.get());
        GMCWriter::reactToAll(newGMCdoc.get(), actions);
        dialogList->close();

        showSelectedGMCResult(GMCResultView->currentIndex());
        currAction = NULL;
    }
}

void appGUI::closeDistNameDialog()
{
    dialogList->close();
}

void appGUI::acceptReportSettings()
{
    if ( genNewRadio->isChecked() )
        reportSettings = ReportSetting("", toBeSorted);
    else
        reportSettings = ReportSetting(templatePath, toBeSorted);

    templatePath = "";
    toBeSorted = false;

    saveDialog->close();
    save();
}

void appGUI::cancelReportSettings()
{
    templatePath = "";
    toBeSorted = false;
    reportSettings = ReportSetting(templatePath, toBeSorted);
    saveDialog->close();
}

void appGUI::loadTemplatePath()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\GMCAutomationTool",
             QSettings::NativeFormat);

    QString openPath = QFileDialog::getOpenFileName(
                this,
                tr("Open Report file"),
                settings.value("Raport_path").toString(),
                tr("HTML (*.html)"));

    settings.setValue("Raport_path", openPath);

    if ( openPath == "" && pathLine->text() == "")
        acceptButton->setEnabled(false);
    else if ( openPath != "")
    {
        pathLine->setText(openPath);
        acceptButton->setEnabled(true);
    }

    templatePath = openPath.toStdString();
}

void appGUI::loadFixPath()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\GMCAutomationTool",
             QSettings::NativeFormat);
    QString openPath = QFileDialog::getOpenFileName(
                this,
                tr("Open file to fix"),
                settings.value("GMC_Fix_Path").toString(),
                tr("XML (*.xml)"));

    if ( openPath != "")
    {
        settings.setValue("GMC_Fix_Path", openPath);
        pathFixLine->setText(openPath);
        accFixButton->setEnabled(true);
    }
    fixFilePath = openPath.toStdString();
}

void appGUI::setToBeSort(bool val)
{
    toBeSorted = val;
}

void appGUI::setFixWithSort(bool val)
{
    fixWithSort = val;
}

void appGUI::radioNewRaport(bool val)
{
    if ( val == true )
    {
        pathLine->setEnabled(false);
        loadPathButton->setEnabled(false);
        acceptButton->setEnabled(true);
    }
    else
    {
        pathLine->setEnabled(true);
        loadPathButton->setEnabled(true);
        if (pathLine->text() == "")
            acceptButton->setEnabled(false);
    }
}

void appGUI::save()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\GMCAutomationTool",
             QSettings::NativeFormat);

    QString savePath = QFileDialog::getSaveFileName(
                this,
                tr("Save New GMC file"),
                settings.value("New_GMC_path").toString(),
                tr("XML (*.xml)"));

    if (savePath.length() > 0)
    {
        settings.setValue("New_GMC_path", savePath);

        newGMCdoc = make_shared<GMCDocument>(oldGMCdoc.get());
        auto report = GMCWriter::reactToAllIncluded(newGMCdoc.get(), actions);
        GMCWriter::updateVersionInGmc(newGMCdoc.get(), newPDDBdoc->getReleaseVersion(), newPDDBdoc->getReleaseName());
        if (XmlWriter::save(newGMCdoc.get()->getXMLDocument(), savePath.toStdString(), reportSettings.isToBeSorted()))
        {
            QDir d = QFileInfo(savePath).absoluteDir();
            QString fName = QFileInfo(savePath).fileName();
            QString logoPath = d.absolutePath() + "/logo.png";
            QString reportPath = savePath;
            reportPath += ".html";
            ReportUtilities::saveLogo(logoPath.toStdString());

            if ( reportSettings.getPath() != "" )
            {
                QFile outFile(QString::fromStdString(reportSettings.getPath()));
                if ( outFile.open(QIODevice::ReadOnly | QIODevice::Text ) )
                {
                    QTextStream in(&outFile);
                    QString templateText = in.readAll();

                    outFile.close();
                    ReportUtilities::generateReportFromExisting(templateText.toStdString(), report, reportPath.toStdString(),
                                                    getFileName(oldPDDBPath), getFileName(newPDDBPath),
                                                    getFileName(oldGMCPath), fName.toStdString(), newPDDBdoc->getReleaseName());
                }
                else
                {
                    QMessageBox::information(this, tr("Saving file"),
                                                   tr("Falied to generate report! Couldn't open report template file         "),
                                                   QMessageBox::Ok);
                }
            }
            else
            {

                ReportUtilities::generateReport(report, reportPath.toStdString(),
                                                getFileName(oldPDDBPath), getFileName(newPDDBPath),
                                                getFileName(oldGMCPath), fName.toStdString(), newPDDBdoc->getReleaseName());
            }
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
    if (item == nullptr)
    {
        return;
    }

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
                    createDistNameDialog();
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

void appGUI::createDistNameDialog()
{
    if (currAction != NULL)
    {
        if (dialogList != NULL)
        {
            delete dialogList;
        }

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

        distListView = new QListView(this);

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
}

void appGUI::setHzVectors()
{
    shared_ptr<GMCDocument> gmc = make_shared<GMCDocument>(hzSettings.getPath());
    if ( gmc->getDocType() == "TDD" )
    {
        shared_ptr<TDDTable> tdd = make_shared<TDDTable>("tdd.csv");
        hzVect = tdd->getHertzList();
        tddFrameConfVect = tdd->getFrameConf();
        tddSpecSubfConfVect = tdd->getSpecSubfConf();
    }
    else if ( gmc->getDocType() == "FDD" )
    {
        if ( hzSettings.getCellType() == "Indoor" )
        {
            shared_ptr<FDDIndoorTable> fdd = make_shared<FDDIndoorTable>("fdd_indoor.csv");
            hzVect = fdd->getHertzList();
        }
        else if ( hzSettings.getCellType() == "Outdoor" )
        {
            shared_ptr<FDDOutdoorTable> fdd = make_shared<FDDOutdoorTable>("fdd_outdoor.csv");
            hzVect = fdd->getHertzList();
        }
    }

    // Tu funckja z
//    hzSettings.getCellType();
//    hzSettings.getPath();
    // Przypisujemy do vektorow
//    hzVect;
//    tddFrameConfVect;

//    tddSpecSubfConfVect;


    setHzCB();
}

void appGUI::setHzCB()
{
    hzCB->clear();

    for ( vector<string>::iterator it = hzVect.begin(); it != hzVect.end(); it++)
        hzCB->addItem(QString::fromStdString(*it));

    if ( isTDD )
    {
        tddFrameConfCB->clear();
        tddSpecSubfConfCB->clear();

        for ( vector<string>::iterator it = tddFrameConfVect.begin(); it != tddFrameConfVect.end(); it++)
            tddFrameConfCB->addItem(QString::fromStdString(*it));

        for ( vector<string>::iterator it = tddSpecSubfConfVect.begin(); it != tddSpecSubfConfVect.end(); it++)
            tddSpecSubfConfCB->addItem(QString::fromStdString(*it));
    }
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
    statusBar()->showMessage(tr("Filling boxes PDDB"));
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

void appGUI::showSelectedGMCResult(QModelIndex index)
{
    if (index.column() != 0)
    {
        statusBar()->showMessage(tr("Filling boxes GMC"));
        //QModelIndex index = GMCResultView->currentIndex();

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
    newGMCTextEdit->setMaximumHeight(this->height() * 0.2);

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

void appGUI::createSaveDialog()
{
    if (!(oldPDDBPath.empty() || newPDDBPath.empty() || oldGMCPath.empty()))
    {
        if (saveDialog != NULL)
        {
            delete saveDialog;
        }

        saveDialog = new QDialog();
        saveDialog->setModal(true);
        saveDialog->setWindowTitle(tr("Save options"));
        saveDialog->setMinimumWidth(300);
        QVBoxLayout * mainLayout = new QVBoxLayout(saveDialog);

        QVBoxLayout * radioButtonLayout = new QVBoxLayout();
        genNewRadio = new QRadioButton(tr("Generate new raport"));
        genFromTamplateRadio = new QRadioButton(tr("Generate raport from template"));

        genNewRadio->setChecked(true);

        connect(genNewRadio, SIGNAL(toggled(bool)), this, SLOT(radioNewRaport(bool)));

        radioButtonLayout->addWidget(genNewRadio);
        radioButtonLayout->addWidget(genFromTamplateRadio);

        QHBoxLayout * loadTemplateLayout = new QHBoxLayout();
        pathLine = new QLineEdit();
        pathLine->setReadOnly(true);
        loadPathButton = new QPushButton(tr("Open file"));

        connect(loadPathButton, SIGNAL(clicked(bool)), this, SLOT(loadTemplatePath()));

        pathLine->setEnabled(false);
        loadPathButton->setEnabled(false);

        loadTemplateLayout->addWidget(pathLine);
        loadTemplateLayout->addWidget(loadPathButton);

        QVBoxLayout * checkboxLayout = new QVBoxLayout();
        QCheckBox * toSort = new QCheckBox(tr("Sort Managed Object"));

        connect(toSort, SIGNAL(clicked(bool)), this, SLOT(setToBeSort(bool)));

        checkboxLayout->addWidget(toSort);

        QHBoxLayout * buttonsLayout = new QHBoxLayout();
        acceptButton = new QPushButton(tr("Save"));
        cancelButton = new QPushButton(tr("Cancel"));

        connect(acceptButton, SIGNAL(clicked(bool)), this, SLOT(acceptReportSettings()));
        connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelReportSettings()));

        buttonsLayout->addWidget(acceptButton);
        buttonsLayout->addWidget(cancelButton);

        mainLayout->addLayout(radioButtonLayout);
        mainLayout->addLayout(loadTemplateLayout);
        mainLayout->addLayout(checkboxLayout);
        mainLayout->addLayout(buttonsLayout);

        saveDialog->setLayout(mainLayout);
        saveDialog->show();
    }
    else
    {
        QMessageBox::warning(this, tr("Saving file"),
                                       tr("PDDB or GMC files not loaded"),
                                       QMessageBox::Ok);
    }
}

void appGUI::createFixDialog()
{
    if (fixDialog != NULL)
    {
        delete fixDialog;

    }
    fixDialog = new QDialog();
    fixDialog->setModal(true);
    fixDialog->setWindowTitle(tr("Fix files"));
    fixDialog->setMinimumWidth(300);
    QGridLayout * mainLayout = new QGridLayout(fixDialog);

    QPushButton * loadPathButton = new QPushButton(tr("Open file"));
    pathFixLine = new QLineEdit();

    pathFixLine->setReadOnly(true);

    mainLayout->addWidget(pathFixLine, 0, 0, 1, 4);
    mainLayout->addWidget(loadPathButton, 0, 4);

    connect(loadPathButton, SIGNAL(clicked(bool)), this, SLOT(loadFixPath()));

    QCheckBox * toSort = new QCheckBox(tr("Sort Managed Objects"));

    connect(toSort, SIGNAL(clicked(bool)), this, SLOT(setFixWithSort(bool)));

    mainLayout->addWidget(toSort, 1, 0);

    accFixButton = new QPushButton(tr("Fix"));
    canFixButton = new QPushButton(tr("Cancel"));

    accFixButton->setEnabled(false);

    connect(accFixButton, SIGNAL(clicked(bool)), this, SLOT(accFixFile()));
    connect(canFixButton, SIGNAL(clicked(bool)), this, SLOT(canFixFile()));

    mainLayout->addWidget(accFixButton, 2, 0, 1, 1);
    mainLayout->addWidget(canFixButton, 2, 2, 1, 3);

    fixDialog->setLayout(mainLayout);
    fixDialog->show();

}

void appGUI::createHzDialog()
{

    QSettings settings("HKEY_CURRENT_USER\\Software\\GMCAutomationTool",
             QSettings::NativeFormat);

    QString filePath = QFileDialog::getOpenFileName(
                this,
                "Open GMC file",
                settings.value("GMC_Variant_Path").toString(),
                tr("XML (*.xml)"));

    if (filePath.length() > 0)
    {
        shared_ptr<GMCDocument> doc = make_shared<GMCDocument>(filePath.toStdString());

        if ( hzDialog != NULL )
        {
            delete hzDialog;
        }

        hzDialog = new QDialog();
        hzDialog->setModal(true);
        hzDialog->setWindowTitle(tr("GMC settings"));
        hzDialog->setMinimumWidth(300);

        hzSettings = HzSettings(filePath.toStdString(), "Indoor");

        QGridLayout * mainLayout = new QGridLayout(hzDialog);
        QLabel * cellTypeLab = new QLabel(tr("Cell Type "));
        cellTypeCB = new QComboBox();

        cellTypeCB->addItem("Indoor");
        cellTypeCB->addItem("Outdoor");
        connect(cellTypeCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(setCellType(QString)));

        QLabel * hzLab = new QLabel(tr("MHz "));
        hzCB = new QComboBox();

        connect(hzCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHz(QString)));

        mainLayout->addWidget(cellTypeLab, 0, 0);
        mainLayout->addWidget(cellTypeCB, 0, 1);
        mainLayout->addWidget(hzLab, 1, 0);
        mainLayout->addWidget(hzCB, 1, 1);

        accHzButton = new QPushButton(tr("Apply"));
        canHzButton = new QPushButton(tr("Cancel"));

        connect(accHzButton, SIGNAL(clicked(bool)), this, SLOT(accHzFile()));
        connect(canHzButton, SIGNAL(clicked(bool)), this, SLOT(canHzFile()));

        isTDD = doc->getDocType()=="TDD";
        if ( isTDD )
        {
            QLabel * tddFrameLab = new QLabel(tr("TDD Frame Conf "));
            tddFrameConfCB = new QComboBox();

            connect(tddFrameConfCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(setTddFrameConfCB(QString)));

            QLabel * tddSpecLab = new QLabel(tr("TDD Spec Subf Conf "));
            tddSpecSubfConfCB = new QComboBox();

            connect(tddSpecSubfConfCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(setTddSpecSubfConfCB(QString)));

            mainLayout->addWidget(tddFrameLab, 2, 0);
            mainLayout->addWidget(tddFrameConfCB, 2, 1);
            mainLayout->addWidget(tddSpecLab, 3, 0);
            mainLayout->addWidget(tddSpecSubfConfCB, 3, 1);

            mainLayout->addWidget(accHzButton, 4, 0);
            mainLayout->addWidget(canHzButton, 4, 1);
        }
        else
        {
            mainLayout->addWidget(accHzButton, 2, 0);
            mainLayout->addWidget(canHzButton, 2, 1);
        }

        setHzVectors();

        hzDialog->setLayout(mainLayout);
        hzDialog->show();
    }
}

void appGUI::createHelpDialog()
{
    QHelpEngine* helpEngine = new QHelpEngine(QApplication::applicationDirPath() +
                                              "/Help/help.qhc");
    helpEngine->setupData();

    QTabWidget* tWidget = new QTabWidget;
    tWidget->setMaximumWidth(200);
    tWidget->addTab(helpEngine->contentWidget(), "Contents");
    tWidget->addTab(helpEngine->indexWidget(), "Index");

    HelpBrowser *textViewer = new HelpBrowser(helpEngine);
    textViewer->setSource(
                QUrl("qthelp://help.qt.helpexample/doc/index.html"));


    connect(helpEngine->contentWidget(),
            SIGNAL(linkActivated(QUrl)),
            textViewer, SLOT(setSource(QUrl)));

    connect(helpEngine->indexWidget(),
            SIGNAL(linkActivated(QUrl, QString)),
            textViewer, SLOT(setSource(QUrl)));

    QSplitter *horizSplitter = new QSplitter(Qt::Horizontal);
    horizSplitter->insertWidget(0, tWidget);
    horizSplitter->insertWidget(1, textViewer);

    helpDialog = new QDialog();
    QVBoxLayout * mainLayout = new QVBoxLayout(helpDialog);
    mainLayout->addWidget(horizSplitter);

    helpDialog->setLayout(mainLayout);
    helpDialog->show();

}

void appGUI::accFixFile()
{
    fixSettings = FixSetting(fixFilePath, fixWithSort);

    tinyxml2::XMLDocument * doc = XmlWrapper::loadDocument(fixSettings.getPath());
    XmlWriter::save(doc, fixSettings.getPath(), fixSettings.isToBeSorted());

    fixDialog->close();
    qDebug() << QString::fromStdString(fixFilePath) << fixWithSort;
    fixFilePath = "";
    fixWithSort = false;

    QMessageBox::information(this, tr("Fixing file"),
                                   tr("Success !         "),
                                   QMessageBox::Ok);
}

void appGUI::canFixFile()
{
    fixFilePath = "";
    fixWithSort = false;

    fixDialog->close();

}

void appGUI::setCellType(QString val)
{

    hzSettings.setCellType(val.toStdString());
    setHzVectors();

}

void appGUI::setHz(QString val)
{

    hzSettings.setHz(val.toStdString());

}

void appGUI::setTddFrameConfCB(QString val)
{

    hzSettings.setFrameConf(val.toStdString());

}

void appGUI::setTddSpecSubfConfCB(QString val)
{

    hzSettings.setSpecSubfConf(val.toStdString());

}

void appGUI::accHzFile()
{
    shared_ptr<GMCDocument> doc = make_shared<GMCDocument>(hzSettings.getPath());
    // tutaj do DOC dajemy settings
    QSettings settings("HKEY_CURRENT_USER\\Software\\GMCAutomationTool",
             QSettings::NativeFormat);



    if (hzSettings.getPath().length() > 0)
    {
        settings.setValue("GMC_Variant_Path", QString::fromStdString(hzSettings.getPath()));
    }


    if ( doc->getDocType() == "TDD" )
    {
        shared_ptr<TDDTable> tdd = make_shared<TDDTable>("tdd.csv");
        GMCWriter::applyGMCVariantForTDD(tdd.get(), doc.get(), hzSettings.getCellType(),
                                         hzSettings.getHz(), hzSettings.getFrameConf(), hzSettings.getSpecSubfConf());
    }
    else if ( doc->getDocType() == "FDD" )
    {
        if ( hzSettings.getCellType() == "Indoor" )
        {
            shared_ptr<FDDIndoorTable> fdd = make_shared<FDDIndoorTable>("fdd_indoor.csv");
            GMCWriter::applyGMCVariantForFDDIndoor(fdd.get(), doc.get(), hzSettings.getHz());
        }
        else if ( hzSettings.getCellType() == "Outdoor" )
        {
            shared_ptr<FDDOutdoorTable> fdd = make_shared<FDDOutdoorTable>("fdd_outdoor.csv");
            GMCWriter::applyGMCVariantForFDDOutdoor(fdd.get(), doc.get(), hzSettings.getHz());
        }
    }

    if (XmlWriter::save(doc->getXMLDocument(), hzSettings.getPath()) )
    {
        hzDialog->close();
        QMessageBox::information(this, tr("GMC Variant"),
                                       tr("Success !                                           "),
                                       QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning(this, tr("GMC Apply Variant"),
                                       tr("Could not save file. Check if the file is available for save     "),
                                       QMessageBox::Ok);
    }



}

void appGUI::canHzFile()
{
    isTDD = false;

    hzDialog->close();
}


void appGUI::setConnections()
{
    GMCResultView->setMouseTracking(true);

    connect(GMCResultView,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(showSelectedGMCResult(QModelIndex)));

    connect(GMCResultView,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(onGMCRClick(const QPoint &)));

    connect(GMCResultView,
            SIGNAL(entered(QModelIndex)),
            this,
            SLOT(setGMCHint(QModelIndex)));

    connect(PDDBResultView,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(showSelectedPDDBResult()));

    connect(PDDBResultView,
            SIGNAL(entered(QModelIndex)),
            this,
            SLOT(setPDDBHint(QModelIndex)));

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
    connect(saveFileAct, SIGNAL(triggered(bool)), this, SLOT(createSaveDialog()));

    displayHelpAct = new QAction(tr("&Help"), this);
    displayHelpAct->setStatusTip(tr("Help"));
    connect(displayHelpAct, SIGNAL(triggered(bool)), this, SLOT(createHelpDialog()));

    openFixAction = new QAction(tr("Format file"), this);
    connect(openFixAction, SIGNAL(triggered(bool)), this, SLOT(createFixDialog()));

    openHzAction = new QAction(tr("GMC select variant"), this);
    connect(openHzAction, SIGNAL(triggered(bool)), this, SLOT(createHzDialog()));

    addToGMC = new QAction(tr("Include in GMC"), this);

    delFromGMC = new QAction(tr("Exclude from GMC"), this);

    setDistName = new QAction(tr("Setup MOC distName in GMC"), this);

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

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(displayHelpAct);
}

void appGUI::createToolBar()
{
    fileToolBar = new QToolBar(tr("&File tool bar"));
    addToolBar(Qt::LeftToolBarArea, fileToolBar);
    fileToolBar->setIconSize(QSize(48, 48));
    openOldPDDBAct->setIcon(QIcon(":/report/opic.png"));
    openNewPDDBAct->setIcon(QIcon(":/report/npic.png"));
    openOldGMCAct->setIcon(QIcon(":/report/ogic.png"));
    saveFileAct->setIcon(QIcon(":/report/sic.png"));
    openFixAction->setIcon(QIcon(":/report/fixic.png"));
    openHzAction->setIcon(QIcon(":/report/hzic.png"));
    fileToolBar->addAction(openOldPDDBAct);
    fileToolBar->addAction(openNewPDDBAct);
    fileToolBar->addAction(openOldGMCAct);
    fileToolBar->addAction(saveFileAct);
    fileToolBar->addAction(openHzAction);
    fileToolBar->addSeparator();

    fileToolBar->addAction(openFixAction);

    viewMenu->addAction(fileToolBar->toggleViewAction());
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
