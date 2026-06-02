#include "mainwindow.h"

#include <QApplication>
#include <QByteArray>
#include <QtGlobal>

int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
#endif

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}