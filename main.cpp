#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icon.png"));

    MainWindow *w = new MainWindow;
    w->show();
    return a.exec();
}
