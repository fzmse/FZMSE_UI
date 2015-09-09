#include "Gui/helpdialog.h"
#include <QtWidgets>

helpDialog::helpDialog()
{
    setMinimumWidth(150);
    setMinimumHeight(100);
    setBaseSize(QSize(200, 300));
    setWindowTitle(tr("Help"));

//    QHBoxLayout *step_1 = new QHBoxLayout();


//    QVBoxLayout *mainLayout = new QVBoxLayout;
//    mainLayout->addLayout(horizontalLayout);
//    mainLayout->addStretch(1);
//    mainLayout->addSpacing(12);
//    mainLayout->addLayout(buttonsLayout);
//    setLayout(mainLayout);
    show();
}

void helpDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{

}
