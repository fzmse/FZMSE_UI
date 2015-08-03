#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow();

private:
    void createMenu();
    void createHorizontalGroupBox();
    void createGridGroupBox();
    void createFormGroupBox();

    enum { NumGridRow = 3, NumButtons = 4 };

    QMenuBar * menuBar;
    QGroupBox * horizontalGroupBox;
    QGroupBox * gridGroupBox;
    QGroupBox * formGroupBox;
    QTextEdit * smallEditor;
    QTextEdit * bigEditor;
    QLabel * labels[NumGridRow];
    QLineEdit * lineEdit[NumGridRow];
    QPushButton * buttons[NumButtons];
    QDialogButtonBox * buttonBox;

    QMenu * fileMenu;
    QAction * exitAction;
    QAction * openFile;

};

#endif // MAINWINDOW_H
