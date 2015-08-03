#include <QtWidgets>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    createMenu();
    createHorizontalGroupBox();
    createGridGroupBox();
    createFormGroupBox();

    bigEditor = new QTextEdit;
    bigEditor->setPlainText(tr("This widget takes up all the remaining space "
                               "in the top-level layout."));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(gridGroupBox);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(bigEditor);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Test app"));
}

void MainWindow::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    openFile = fileMenu->addAction(tr("Open file"));
    exitAction = fileMenu->addAction(tr("&Exit"));
    menuBar->addMenu((fileMenu));

    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(accept()));
}

void MainWindow::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Horizontal group box"));
    QHBoxLayout * layout = new QHBoxLayout;

    for (int i = 0; i < NumButtons; ++i)
    {
        buttons[i] = new QPushButton(tr("Button %1").arg(i+1));

        layout->addWidget(buttons[i]);

    }
    horizontalGroupBox->setLayout(layout);
}

void MainWindow::createGridGroupBox()
{
    gridGroupBox = new QGroupBox(tr("Grid group Box"));
    QGridLayout * layout = new QGridLayout;

    for (int i = 0; i < NumGridRow; ++i)
    {
        labels[i] = new QLabel(tr("line %1").arg(i+1));
        lineEdit[i] = new QLineEdit;
        layout->addWidget(labels[i], i+1, 0);
        layout->addWidget(lineEdit[i], i+1, 1);
    }

    smallEditor = new QTextEdit;
    smallEditor->setPlainText(tr("Tu bedzie maly test"
                                 "i tutaj tez"));
    layout->addWidget(smallEditor, 0, 2, 4, 1);

    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);
    gridGroupBox->setLayout(layout);
}

void MainWindow::createFormGroupBox()
{
    formGroupBox = new QGroupBox(tr("Form layout"));
    QFormLayout * layout = new QFormLayout;
    layout->addRow(new QLabel(tr("Line 1:")), new QLineEdit);
    layout->addRow(new QLabel(tr("Line 2:")), new QComboBox);
    layout->addRow(new QLabel(tr("Line 3:")), new QSpinBox);
    formGroupBox->setLayout(layout);
}
