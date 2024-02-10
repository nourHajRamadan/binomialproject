#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;

    w.resize(STDW,STDH);
    w.show();

    w.firstWelcome();
    w.offerUpdate();

    return a.exec();

}
