#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;

    w.getJsonVariables();
    std::thread t1(&MainWindow::preparePopUps,&w);

    w.resize(STDW,STDH);
    w.show();

    w.firstWelcome();

    while(w.offerUpdate()){}
    t1.join();
    w.showSpecialmessage();

    return a.exec();

}
