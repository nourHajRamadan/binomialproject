#include "mainwindow.h"

#include <QApplication>
#include <QLocale>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;

    w.resize(STDW,STDH);
    w.show();

    w.firstWelcome();

    return a.exec();

}
