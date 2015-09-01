#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class helpDialog : public QDialog
{
    Q_OBJECT

public:
    helpDialog();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
   void createHelp();

   QListWidget * steps;
   QStackedWidget * pagesWidget;
};
