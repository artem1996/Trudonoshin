#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    std::cout.sync_with_stdio(false);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
