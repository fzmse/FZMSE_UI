#include "Gui/appGui.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    appGUI w;
    w.show();

    return app.exec();
}
