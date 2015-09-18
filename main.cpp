#include "Gui/appGui.h"
#include <QApplication>

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    appGUI w;
    w.show();
    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    return app.exec();
}
