#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;

    w.resize(STDW,STDH);
    w.show();

    w.firstWelcome();
    while(w.offerUpdate()){}
    //w.showSpecialmessage();

    return a.exec();

}
