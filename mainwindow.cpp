#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mathstuff.cpp"
#include "htmlstuff.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"Initiating Binomialrechner v"+currentVersion;

    ui->jsonResetButton->hide();//tmp

//Standard: name of this session's folder based on date and time
    QDateTime q=q.currentDateTime();//OG "Mi Feb 28 01:14:55 2024"
    QString sesh = q.toString();
    //Sitzung am 28.Feb.2024 um 01.14Uhr
    sessionDocname=QString("%1. %2. %3 um %4.%5Uhr").arg(sesh.section(32,2,2)).arg(sesh.section(32,1,1)).arg(sesh.section(32,4,4)).arg(QString(sesh.section(32,3,3)).section(':',0,0)).arg(QString(sesh.section(32,3,3)).section(':',1,1));

//Standard: all tabs closed except for welcome
    for(int i=1;i<=4;i++){
        ui->tabWidget->setTabVisible(i,0);
    }

//Standard: mit n und p rechnen
    ui->_mu_label->hide();
    ui->_sigma_label->hide();
    ui->_m_edit->hide();
    ui->_sigma_edit->hide();

//Standard: mit binompdf rechnen
    ui->upperEdit->hide();
    ui->upperLabel->hide();
    ui->underLabel->setText("Ereignis");
    ui->underEdit->setPlaceholderText("k");

//Standard: Intervalloptionen und Optionensichtbarkeit
    ui->std_choose_marked_interval->addItems({"Konfidenzintervall aussuchen","1σ-Intervall","2σ-Intervall","3σ-Intervall","90%-Intervall","95%-Intervall","99%-Intervall"});
    ui->std_choose_marked_interval->hide();
    ui->std_show_histo_markings->hide();
    ui->showPNGTarget->hide();
    ui->saveCSV->hide(); ui->savePNG->hide();
    ui->tSt1->hide(); ui->tSt2->hide(); ui->tSt3->hide();
    ui->option_label->hide();

//PRK Standard Settings
    ui->prk_targetParameter->addItems({"Stichprobenumfang n","Trefferquote p","Ereignis k"});
    ui->prk_2nd_compStatement->addItems({"≥","≤"});

//Instruction Tab Settings
    ui->instructionsTo->addItems({"dem allgemeinen Umgang","dem Standardrechner","den wichtigen Funktionen","dem Parameter Retrieval Kit","Aktualisierung des Binomialrechners"});   

//Palette Settings
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QColor darkGray(53, 53, 53);
    QColor gray(128, 128, 128);
    QColor black(25, 25, 25);

    //experimental
    //QColor blue(145, 19, 209);
    //QColor blue(QRandomGenerator::global()->bounded(0,255),QRandomGenerator::global()->bounded(0,255),QRandomGenerator::global()->bounded(0,255));
    QColor azure(0x007FFF);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, darkGray);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, black);
    darkPalette.setColor(QPalette::AlternateBase, darkGray);
    darkPalette.setColor(QPalette::ToolTipBase, azure);
    darkPalette.setColor(QPalette::ToolTipText, Qt::black);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, darkGray);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Link, azure);
    darkPalette.setColor(QPalette::Highlight, azure);
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    darkPalette.setColor(QPalette::Active, QPalette::Button, gray.darker());
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Light, darkGray);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

}

MainWindow::~MainWindow()
{
    delete ui;
}

/////mainwindow.cpp overview:
/// 1. tabWidget Signals
/// 2. Organisatory Block
/// 3. Standardrechner Block
/// 4. show Histogramm Block
/// 5. show Table Block
/// 6. Wichtige Funktionen Block
/// 7. Parameter Retrieval Kit Block
/// 8. Lucky Block

///// ------ <<<<<tabWidget Signals>>>>> ------

inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if(!index){
        this->resize(STDW,STDH);
        welcomeDisplay();
        return;
    }
    QString tmp=ui->tabWidget->tabText(index);
    if((tmp=="Standardrechner" or tmp=="Wichtige Funktionen" or tmp=="Parameter Retrieval Kit" or tmp=="Gebrauchsanweisungen")){
        ui->tabWidget->setTabVisible(index,0);
    }else{
        ui->tabWidget->removeTab(index);
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(!index&&!running){
        ui->tabWidget->setCurrentIndex(0);
        signal=0;
        changeSizeEvent();
    }
}

void MainWindow::on_show_std_button_clicked()
{
    ui->_n_edit->setFocus();
    ui->tabWidget->setTabVisible(1,1);
    ui->tabWidget->setCurrentIndex(1);
    !easteregg?easteregg++:easteregg=0;
}

void MainWindow::on_show_wf_button_clicked()
{
    if(easteregg==4){
        qDebug()<<"Easteregg!";
        int i=30;
        this->resize(STDW,915);
        this->move(400,50);
        while(i){
            QString arg,pre = QString(R"(<br><span style="font-size: 152px; font-family: Jokerman; color: rgb(%1, %2, %3);">)").arg(QRandomGenerator::global()->bounded(255)).arg(QRandomGenerator::global()->bounded(255)).arg(QRandomGenerator::global()->bounded(255));
            arg.push_back(QString("%1%2%3").arg(pre).arg("0000000000").arg("</span>"));
            arg.push_back(QString("%1%2%3").arg(pre).arg("0 Easteregg 0").arg("</span>"));
            arg.push_back(QString("%1%2%3").arg(pre).arg("0000000000").arg("</span>"));
            ui->textBrowserX->setHtml(arg);
            delay(2000);
            if(!(28-i)) QDesktopServices::openUrl (QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
            i--;
        }
        easteregg=0;
        return;
    }else{easteregg=0;}
    ui->wf_factorial_edit->setFocus();
    ui->tabWidget->setTabVisible(2,1);
    ui->tabWidget->setCurrentIndex(2);
}


void MainWindow::on_show_prk_button_clicked()
{
    easteregg==3?easteregg++:easteregg=0;
    ui->tabWidget->setTabVisible(3,1);
    ui->tabWidget->setCurrentIndex(3);
}

void MainWindow::on_show_manuel_button_clicked()
{
    easteregg==1?easteregg++:easteregg=0;
    ui->tabWidget->setTabVisible(4,1);
    ui->tabWidget->setCurrentIndex(4);
}

////-----------end of block-----------

///// ------ <<<<<Organisatory Block>>>>> ------

void MainWindow::resizeEvent(QResizeEvent* event){
    currentSTDW=event->size().width();
    currentSTDH=event->size().height();
    qDebug()<<"width:"<<currentSTDW<<"\t";
    qDebug()<<"height:"<<currentSTDH<<"\n";
    if((currentSTDW==(sysXmax) or currentSTDH==(sysYmax-40))&&!running){
        signal=0;
        changeSizeEvent();
    }
}

QString getDocPath(){
    QString res, docdir(getenv("USERPROFILE"));
    for(int i=0;i<docdir.length();i++){
        if(docdir[i]==(char)92){
            res.append('/');
        }else{
            res.append(docdir[i]);
        }
    }
    res.append("/Documents/Binomialrechner");
    return res;
}

void MainWindow::getJsonVariables(){

    QString path=getDocPath();
    path.append("/config");
    QDir dir(path);
    if (!dir.exists()) dir.mkpath(".");
    QFile file(path+"/config.json");
    if( file.open( QIODevice::ReadOnly ) ){
        QByteArray bytes=file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
        if( jsonError.error != QJsonParseError::NoError )
        {
            qDebug()<<"fromJson failed: "<<jsonError.errorString();
            return ;
        }
        if(document.isObject()){
            QJsonObject jsonObj=document.object();
            if(jsonObj.contains("hideUpdate")){
                auto value=jsonObj.take("hideUpdate");
                if(value.isDouble())jsonHideUpdates=value.toInt();
            }
            if(jsonObj.contains("hideSpecial")){
                auto value=jsonObj.take("hideSpecial");
                if(value.isDouble())jsonHideSpecialmessages=value.toInt();
            }
        }
    }

    qDebug()<<"[JSON] u:"<<jsonHideUpdates<<" s:"<<jsonHideSpecialmessages;
}

void MainWindow::setJsonVariables(){

    QString path=getDocPath();
    path.append("/config");
    QDir dir(path);
    if (!dir.exists()) dir.mkpath(".");
    QFile file(path+"/config.json");
    QJsonObject popup;
    popup.insert("hideUpdate",jsonHideUpdates);
    popup.insert("hideSpecial",jsonHideSpecialmessages);
    QJsonDocument document;
    document.setObject(popup);
    QByteArray bytes=document.toJson(QJsonDocument::Indented);
    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
        iStream.setCodec( "utf-8" );
        iStream << bytes;
        file.close();
    }
    else
    {
        qDebug()<<"file open failed: "<<path;
    }

}

void MainWindow::on_jsonResetButton_clicked()//tmp
{
    jsonHideUpdates=jsonHideSpecialmessages=0;
    setJsonVariables();
    preparePopUps();
    while(offerUpdate()){}
    showSpecialmessage();
}

void MainWindow::on_instructionsTo_currentIndexChanged(int index)
{
    ui->instructionsBrowser->setOpenExternalLinks(1);
    switch (index){
    case 0:{
        ui->instructionsBrowser->setHtml(general_ins);
        break;
    }
    case 1:{
        ui->instructionsBrowser->setHtml(stdCal_ins);
        break;
    }
    case 2:{
        ui->instructionsBrowser->setHtml(wf_ins);
        break;
    }
    case 3:{
        ui->instructionsBrowser->setHtml(prk_ins);
        break;
    }
    case 4:{
        ui->instructionsBrowser->setHtml(update_ins);
        if(jsonHideUpdates or jsonHideSpecialmessages){ ui->jsonResetButton->show();}else{ui->jsonResetButton->hide();}//tmp
        break;
    }
    default:{
        ui->instructionsBrowser->setHtml(general_ins);
        break;

    }
    }
}

void MainWindow::preparePopUps(){
    qDebug()<<"started preparing PopUps!";
    if(!jsonHideUpdates){
    int i=5;
    while(updateinfo.length()<50&&i){
        fetchTextfeed(1,"https://raw.githubusercontent.com/nourHajRamadan/playground/main/up8426xhd2.html");
        delay(500);
        i--;
    }
    }
    if(!jsonHideSpecialmessages){
    int i=5;
    while(specialmessage.length()<7&&i){
        fetchTextfeed(2,"https://raw.githubusercontent.com/nourHajRamadan/playground/main/sm94rud334.html");
        delay(500);
        i--;
    }
    }
    qDebug()<<"finished preparing PopUps!";
}

QString zeug,zeug2;
int MainWindow::offerUpdate(){ //int return because of while in main.cpp
    if(updateinfo.length()<50)return 0;
    QString newst=updateinfo.chopped(updateinfo.length()-6);
    newst.remove("v");
    newst.remove(".");
    currentVersion.remove(".");
    if(newst.toInt()>currentVersion.toInt()){

        QMessageBox mes;
        QString feed=QString(R"(<p><span style="font-family: Veranda, serif; font-size: 20px;"><strong>Es gibt eine neue Version des Binomialrechners zum Download verf&uuml;gbar!</strong></span></p>
            <p><span style="font-family: Veranda, serif; font-size: 20px;"><strong>&Uuml;bersicht der neuen Features:</strong></span></p>)");

        mes.setWindowTitle("Binomialrechner aktualisieren");
        mes.setFont(QFont("Courier New",12,1,0));
        mes.setTextFormat(Qt::RichText);
        mes.setText(feed);
        mes.setInformativeText(updateinfo);

        mes.addButton("Zur Downloadseite",QMessageBox::AcceptRole);
        mes.addButton("Später",QMessageBox::RejectRole);
        mes.addButton("Nicht mehr anzeigen",QMessageBox::ApplyRole);//->3
        int tmp=mes.exec();
        if(!tmp)QDesktopServices::openUrl(QUrl("https://rwth-aachen.sciebo.de/s/3xlkX0pkpaNCkDz"));
        if(tmp==2){
            jsonHideUpdates++;
            setJsonVariables();
        }
    }
    return 0;
}

void MainWindow::showSpecialmessage(){
    //specialmessage=QString(R"(0$51$3$caveat$Heute ist der 03.14.2024!$Happy Pi Day!!  $https://de.wikipedia.org/wiki/Pi-Tag)");//showBool/Size/Width/Fontfamily/Windowtitle/Headline/Infotext
    if(specialmessage.length()<7)return;
    int showBool=QString(specialmessage.section('$',0,0)).toInt();
    if(!showBool)return;
    int size=QString(specialmessage.section('$',1,1)).toInt();
    int width=QString(specialmessage.section('$',2,2)).toInt();
    QString fontfamily=specialmessage.section('$',3,3);
    QString windowtitle=specialmessage.section('$',4,4);
    QString headline=specialmessage.section('$',5,5);
    QString infolink=specialmessage.section('$',6,6);
    //qDebug()<<QString("%1 %2 %3 %4 %5 %6 %7\n").arg(showBool).arg(size).arg(width).arg(fontfamily).arg(windowtitle).arg(headline).arg(infolink);

    QMessageBox mes;
    mes.setWindowTitle(windowtitle);
    mes.setFont(QFont(fontfamily,size,width,0));
    mes.setText(headline);
    mes.addButton("Cool",QMessageBox::RejectRole);
    mes.addButton("Mehr erfahren",QMessageBox::HelpRole);//->1
    mes.addButton("Nicht mehr anzeigen",QMessageBox::ApplyRole);//->2
    int tmp=mes.exec();
    if(tmp==1)QDesktopServices::openUrl(QUrl(infolink));
    if(tmp==2){
        jsonHideSpecialmessages++;
        setJsonVariables();
    }

}
int idextern;
void MainWindow::fetchTextfeed(int id, QString s)
{
    idextern=id;
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    QNetworkReply* rep = manager->get(QNetworkRequest(QUrl(s)));

    QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply) {
        if (reply->error())
        {
            qDebug() << "ERROR!";
            qDebug() << reply->errorString();
        }
        else
        {
            auto data = reply->readAll();
            if(idextern==1)zeug=data;
            if(idextern==2)zeug2=data;
            zeug=data;
        }
    });

    if(id==1)updateinfo=zeug;
    if(id==2)specialmessage=zeug2;

    QObject::connect(manager, &QNetworkAccessManager::finished, manager,&QNetworkAccessManager::deleteLater);
    QObject::connect(manager, &QNetworkAccessManager::finished, rep, &QNetworkReply::deleteLater);

}

////-----------end of block-----------

////////  -------- <<<<< Standardrechner >>>>> -----------

QString specialDoubleInput(QString arg){
    if(arg.contains(',')){
        arg.replace(',','.');
        return arg;
    }else if(arg.contains('/')){
        int signal=0;
        if(arg!=NULL && arg[arg.length()-1]==(char)32){
            arg.remove(' ');
            signal++;
        }
        QString a=arg.section('/',0,0);
        QString b=arg.section('/',1,2);
        double res=a.toDouble()/b.toDouble();
        if(res>pow(2,30))res=0;
        QString tmp=QString::number(res);

        return !signal?tmp:tmp+" ";

    }else{
        return arg;
    }
}

void MainWindow::on_std_show_options_clicked(bool checked)
{
    if(checked){
        ui->std_show_options->hide();
        ui->std_choose_marked_interval->show();
        ui->std_show_histo_markings->show();
        ui->showPNGTarget->show();
        ui->saveCSV->show(); ui->savePNG->show();
        ui->tSt1->show(); ui->tSt2->show(); ui->tSt3->show();
        ui->option_label->show();
    }
}

void MainWindow::on_calWithParam_clicked(bool checked)
{
    if(n>0&&p>0&&mu>0&&sigma>0){//assures proper display of parameter upon switch
        ui->_n_edit->setText(QString::number(n));
        ui->_p_edit->setText(QString::number(p));
        ui->underEdit->setText(QString::number(k1));
        ui->upperEdit->setText(QString::number(k2));
        ui->_m_edit->setText(QString::number(mu));
        ui->_sigma_edit->setText(QString::number(sigma));
    }else{
        n=p=k1=k2=mu=sigma=0;
        ui->_n_edit->setText("");
        ui->_p_edit->setText("");
        ui->underEdit->setText("");
        ui->upperEdit->setText("");
        ui->_m_edit->setText("");
        ui->_sigma_edit->setText("");
    }

    if (checked){//mit Mu und Sigma rechnen >en>> calculate with mu and sigma
        ui->_n_label->hide();
        ui->_p_label->hide();
        ui->_n_edit->hide();
        ui->_p_edit->hide();

        ui->_mu_label->show();
        ui->_sigma_label->show();
        ui->_m_edit->show();
        ui->_sigma_edit->show();

        setToParam=2;

        ui->_m_edit->setFocus();

    }else{//mit N und P rechnen >en>> calculate with n and p
        ui->_n_label->show();
        ui->_p_label->show();
        ui->_n_edit->show();
        ui->_p_edit->show();

        ui->_mu_label->hide();
        ui->_sigma_label->hide();
        ui->_m_edit->hide();
        ui->_sigma_edit->hide();

        setToParam=1;

        ui->_n_edit->setFocus();
    }

    binomOutput(setToCumulative,setToParam);
}

void MainWindow::on_cumulativeP_clicked(bool checked)
{
    if (!checked){//binompdf -->default
        ui->upperEdit->hide();
        ui->upperLabel->hide();
        ui->underLabel->setText("Ereignis");
        ui->underEdit->setPlaceholderText("k");
        setToCumulative=1;
        ui->underEdit->setFocus();
        ui->upperEdit->setText("");
        k2=0;
    }
    if(checked){//binomcdf
        ui->upperEdit->show();
        ui->upperLabel->show();
        ui->underLabel->setText("Untere Grenze");
        ui->underEdit->setPlaceholderText(" ");
        setToCumulative=2;
        ui->upperEdit->setFocus();
    }
    binomOutput(setToCumulative,setToParam);
}

//j=1 --> binompdf  j=2 --> binomcdf
//y=1 --> mit N und P rechnen y=2 --> mit Mu und Sigma rechnen
void MainWindow::binomOutput(int j, int y){

    if(y==1){
        if(n==0 or p==0 or fromNandP(n,p,mu,sigma)!=0){
            sthsWrong();
        }else{
        fromNandP(n,p,mu,sigma);
        ui->hiddenOutput->setText(QString("Erwartungswert μ=%1\nStandardabweichung  σ=%2").arg(QString::number(mu)).arg(QString::number(sigma)));
        }
    }
    if(y==2){
        if(mu==0 or sigma==0 or fromMuandSigma(mu,sigma,n,p)!=0){
            sthsWrong();
        }else{
        fromMuandSigma(mu,sigma,n,p);
        ui->hiddenOutput->setText(QString("Stichprobenumfang n=%1\nTrefferwahrscheinlichkeit p=%2").arg(n).arg(p));
        }
    }
    if(j==1){
        if(binompdf(n,p,k1)==-1){
            sthsWrong();
        }else if(!shitstorm){
            long double resnum =binompdf(n,p,k1);
            QString res =QString::number(resnum,'f',10);
            QString res2 =QString::number(resnum,'f',4);
            QString res3 =QString::number(resnum*100,'f',2);
            ui->pOutput->setText(QString("P(X=%1) = %2\n\t≈ %3 = %4\%").arg(k1).arg(res).arg(res2).arg(res3));
        }else{ui->pOutput->setText(" "); shitstorm=0;}
    }
    if(j==2){
        if(binomcdf(n,p,k1,k2)==-1){
            sthsWrong();
        }else{
            long double resnum = binomcdf(n,p,k1,k2);
            QString res =QString::number(resnum,'f',10);
            QString res2 =QString::number(resnum,'f',4);
            QString res3 =QString::number(resnum*100,'f',2);
            if(!k1){//höchstens -- at most
                ui->pOutput->setText(QString("P(X≤%1) = %2\n\t≈ %3 = %4\%").arg(k2).arg(res).arg(res2).arg(res3));
            }else if(k2==n){//mindestens -- at least
                ui->pOutput->setText(QString("P(X≥%1) = %2\n\t≈ %3 = %4\%").arg(k1).arg(res).arg(res2).arg(res3));
            }else if(k1==k2){//identische Grenzen -- identical limits
                ui->pOutput->setText(QString("P(X=%1) = %2\n\t≈ %3 = %4\%").arg(k1).arg(res).arg(res2).arg(res3));
            }else{//beidseitig -- two different limits
                ui->pOutput->setText(QString("P(%1≤X≤%2) = %3\n\t≈ %4 = %5\%").arg(k1).arg(k2).arg(res).arg(res2).arg(res3));
            }

        }
    }
    //if(markIndex)on_std_choose_marked_interval_currentIndexChanged(markIndex);

}

void MainWindow::sthsWrong(){
    ui->hiddenOutput->setText("Ungültige/unvollständige Eingabe!");
    shitstorm=0;
    ui->pOutput->setText("");
}


void MainWindow::on__n_edit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32 && setToParam==1){
        arg.remove(' ');
        ui->_n_edit->setText(arg);
        ui->_p_edit->setFocus();
    }
    n=arg.toInt()>=0?arg.toInt():0;
    binomOutput(setToCumulative,setToParam);
    if(markIndex){
        on_std_choose_marked_interval_currentIndexChanged(markIndex);
        ui->_n_edit->setFocus();
    }

}


void MainWindow::on__p_edit_textChanged(const QString &arg1)
{
    QString arg=specialDoubleInput(arg1);
    if(arg!=NULL && arg[arg.length()-1]==(char)32 && setToParam==1){
        arg.remove(' ');
        ui->_p_edit->setText(arg);
        ui->underEdit->setFocus();
    }
    p=arg.toDouble()>=0?arg.toDouble():0;
    binomOutput(setToCumulative,setToParam);
    if(markIndex){
        on_std_choose_marked_interval_currentIndexChanged(markIndex);
        ui->_p_edit->setFocus();
    }
}


void MainWindow::on_underEdit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->underEdit->setText(arg);
        if(setToParam==1) setToCumulative==1? ui->_n_edit->setFocus():ui->upperEdit->setFocus();
        if(setToParam==2) setToCumulative==1? ui->_m_edit->setFocus():ui->upperEdit->setFocus();
    }

    k1=arg.toInt()>=0?arg.toInt():0;
    if(!k1 && arg!="0") shitstorm=1;
    binomOutput(setToCumulative,setToParam);
    if(k1!=c1 or k2!=c2)ui->std_choose_marked_interval->setCurrentIndex(0);
}


void MainWindow::on_upperEdit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->upperEdit->setText(arg);
        setToParam==1?ui->_n_edit->setFocus():ui->_m_edit->setFocus();
    }
    k2=arg.toInt()>=0?arg.toInt():0;
    if(!k2 && arg!="0") shitstorm=1;
    binomOutput(setToCumulative,setToParam);
    if(k1!=c1 or k2!=c2)ui->std_choose_marked_interval->setCurrentIndex(0);
}

void MainWindow::on__m_edit_textChanged(const QString &arg1)
{
    QString arg=specialDoubleInput(arg1);
    if(arg!=NULL && arg[arg.length()-1]==(char)32 && setToParam==2){
        arg.remove(' ');
        ui->_m_edit->setText(arg);
        ui->_sigma_edit->setFocus();
    }
    mu=arg.toDouble()>=0?arg.toDouble():0;
    binomOutput(setToCumulative,setToParam);
    if(markIndex){
        on_std_choose_marked_interval_currentIndexChanged(markIndex);
        ui->_m_edit->setFocus();
    }
}


void MainWindow::on__sigma_edit_textChanged(const QString &arg1)
{
    QString arg=specialDoubleInput(arg1);
    if(arg!=NULL && arg[arg.length()-1]==(char)32 && setToParam==2){
        arg.remove(' ');
        ui->_sigma_edit->setText(arg);
        ui->underEdit->setFocus();
    }
    sigma=arg.toDouble()>=0?arg.toDouble():0;
    binomOutput(setToCumulative,setToParam);
    if(markIndex){
        on_std_choose_marked_interval_currentIndexChanged(markIndex);
        ui->_sigma_edit->setFocus();
    }
}

///////// ----------- end of block -----------

///////// ----- <<<<<Show Histogramm>>>>> ------

void MainWindow::on_savePNG_clicked(bool checked)
{
    if(checked){hSt=1;histoAni=0;}else{hSt=0;histoAni=1;}
}

void pngexport(QChartView* chartView, int histocount, QString sessionDocname){

    QPixmap p = chartView->grab();
    QOpenGLWidget *glWidget  = chartView->findChild<QOpenGLWidget*>();
    if(glWidget){
        QPainter painter(&p);
        QPoint d = glWidget->mapToGlobal(QPoint())-chartView->mapToGlobal(QPoint());
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.drawImage(d, glWidget->grabFramebuffer());
        painter.end();
    }

    QString docPath=getDocPath();
    QDir dir(QString("%1/Sitzung am %2").arg(docPath).arg(sessionDocname));
    if (!dir.exists()) dir.mkpath(".");
    qDebug()<<"PNG save:"<<p.save(QString("%1/%2/histogramm no.%3.png").arg(docPath).arg(dir.dirName()).arg(histocount));
}

void MainWindow::on_showPNGTarget_clicked()
{
    QDesktopServices::openUrl( QUrl::fromLocalFile(QString("%1/Sitzung am %2").arg(getDocPath()).arg(sessionDocname)));
}

void MainWindow::on_std_show_histo_markings_clicked(bool checked)
{
    if(checked){
        histoMarkings++;
    }else{
        histoMarkings=0;
    }
}

void MainWindow::getConfidence(int isSigma, double target){
    this->on_cumulativeP_clicked(1);
    ui->cumulativeP->setCheckState(Qt::Checked);
    if(isSigma){
        double lower=mu-(target*sigma);
        double higher=mu+(target*sigma);
        c1=k1=ceil(lower);
        c2=k2=floor(higher);
    }else{
        if(intervalAroundMu(n,p,target,k1,k2)==-1){sthsWrong(); return;}
        c1=k1;  c2=k2;
    }

    ui->underEdit->setText(QString::number(k1));
    ui->upperEdit->setText(QString::number(k2));
}

void MainWindow::on_std_choose_marked_interval_currentIndexChanged(int index)
{
    markIndex=index;
    switch(index){
    case 0:
    {
        return;
        break;
    }
    case 1://1s
    {
        getConfidence(1,1);
        break;
    }
    case 2://2s
    {
        getConfidence(1,2);
        break;
    }
    case 3://3s
    {
        getConfidence(1,3);
        break;
    }
    case 4://90%
    {
        getConfidence(0,0.9);
        break;
    }
    case 5://95%
    {
        getConfidence(0,0.95);
        break;
    }
    case 6://99%
    {
        getConfidence(0,0.99);
        break;
    }
    }
}

void MainWindow::on_showHisto_clicked()
{
    QMessageBox mes;
    mes.setWindowTitle("Warnung");
    mes.setText("Keine sinnvoll darstellbare Histogramme für n und p!\t");
    if(n==0 or p==0 or n>=30000 or p>=1 or (setToCumulative==2&&k2<k1&&histoMarkings)){
        mes.exec();
        return;
    }
    QString title = QString("Binomialverteilung für n=%1 p=%2\n(μ=%3  σ=%4)").arg(n).arg(p).arg(mu).arg(sigma);
    QBarSet* distoOther = new QBarSet("o");
    QBarSet* distoMain = new QBarSet("m");
    QBarSet* distoNull = new QBarSet("n");
    if(histoMarkings){
    if(setToCumulative==2&&k1>=0&&k2>0&&k2>k1){//binomcdf
        title+=QString("     Markierung für P(%1≤X≤%2)=%3").arg(k1).arg(k2).arg(binomcdf(n,p,k1,k2));
        for (int i = 0; i <= n; ++i) {
            if(i<k1 or i>k2){distoOther->append(binompdf(n,p,i));}else{distoOther->append(0);}
            if(i>=k1&&i<=k2){distoMain->append(binompdf(n,p,i));}else{distoMain->append(0);}
            distoNull->append(0);
        }
    }
    if(setToCumulative==1&&k1>0){//binompdf
        title+=QString("     Markierung für P(X=%1)=%2").arg(k1).arg(binompdf(n,p,k1));
        for (int i = 0; i <= n; ++i) {
            if(i!=k1){distoOther->append(binompdf(n,p,i));}else{distoOther->append(0);}
            if(i==k1){distoMain->append(binompdf(n,p,i));}else{distoMain->append(0);}
            distoNull->append(0);
        }
    }
    }
    if((k1<=0&&k2<=0) or !histoMarkings){//default
        for (int i = 0; i <= n; ++i) {
            distoOther->append(0);
            distoMain->append(0);
            distoNull->append(binompdf(n,p,i));
        }
    }
    QStackedBarSeries *series = new QStackedBarSeries();
    series->setBarWidth(1);
    series->append(distoNull);//dark brown
    series->append(distoOther);//bright brown
    series->append(distoMain);//firey red

    QChart *chart = new QChart();
    chart->setTitle(title);
    chart->legend()->hide();
    chart->addSeries(series);
    if(histoAni){
        chart->setAnimationOptions(QChart::AllAnimations);
    }else{
        chart->setAnimationOptions(QChart::NoAnimation);
    }

    auto axisX = new QValueAxis;

    if(n<1221){
      axisX->setRange(0,n);
      axisX->setTickCount(perfectspacing(n)+1);
    }else{
      axisX->setRange(round(mu-5*sigma),round(mu+5*sigma));
      axisX->setTickCount(perfectspacing(round(mu+5*sigma)-round(mu-5*sigma))+1);
      mes.setText("    x-Achse stellt nur noch Werte im 5-σ Intervall dar!\t\n\tDarstellung bis n=30000 möglich.\t");
      mes.exec();
    }

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto axisY = new QValueAxis;
    axisY->setRange(0,binompdf(n,p,round(mu))+0.005);

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->adjustSize();
    chartView->chart()->setTheme(QChart::ChartThemeBrownSand);

    int c=ui->tabWidget->count();
    ui->tabWidget->insertTab(c,chartView,QString(">>Histo No.%1").arg(++histocount));
    ui->tabWidget->setCurrentIndex(c);

    if(hSt){
      if(n>150 && tipSt){
            mes.setWindowTitle("Tipp!");
            mes.setText("Vergrößern Sie das Fenster für bessere Bilder bei großem n!\t");
            mes.exec();
            tipSt=0;
      }
      pngexport(chartView,histocount,sessionDocname);
    }
}
////-----------end of block-----------

///////// ----- <<<<<Show Table>>>>> ------

void MainWindow::on_tSt1_clicked(bool checked)
{
    if(checked){tSt1=1;}else{tSt1=0;}
}


void MainWindow::on_tSt2_clicked(bool checked)
{
    if(checked){tSt2=1;}else{tSt2=0;}
}


void MainWindow::on_tSt3_clicked(bool checked)
{
    if(checked){tSt3=1;}else{tSt3=0;}
}

void MainWindow::on_saveCSV_clicked(bool checked)
{
    if(checked){tSSt=1;}else{tSSt=0;}
}

void MainWindow::on_showTable_clicked()
{
    QMessageBox mes;
    mes.setWindowTitle("Warnung");
    if(n<=2 or p==0 or p>=1 or (n>50000&&((tSt1&&tSt2&&tSt3) or tSt2 or tSt3)) or(!tSt1&&!tSt2&&!tSt3) or(n>100000&&tSt1&&!tSt2&&!tSt3)){
        mes.setText("Keine sinnvoll darstellbare Tabellen für n und p!\t");
        mes.exec();
        return;
    }
    if(n>=5000){
        mes.setText("Ab n=5000 könnte die Erstellung der Tabelle etwas Zeit in Anspruch nehmen.\n(Maximaler Wert für die Erstellung aller Tabellen: n=50000)\nBitte seien Sie geduldig!\t");
        mes.exec();
    }

    //auto tableWidget = new QTableWidget(n+1, 5, this);
    QStringList nullist,col0,col1,col2,col3,col4,headers;
    int cntr=1;
    headers.append("k");
    if(tSt1){ headers.append("P(X=k)");  cntr++;}
    if(tSt2){ headers.append("P(X≤k)");  cntr++;}
    if(tSt3){ headers.append("P(X≥k)");  cntr++;}
    headers.append("Eckdaten"); cntr++;
    auto tableWidget = new QTableWidget(n+1, cntr, this);
    tableWidget->setHorizontalHeaderLabels(headers);
    QFont sansFont("Helvetica [Cronyx]", 11);
    for (int i=0; i<=n; ++i) {//numerating and setting up
        nullist.append("");
        tableWidget->setItem(i, 0, new QTableWidgetItem(tr(" %1 ").arg(i)));
        if(tSSt)col0.append(QString::number(i));
        tableWidget->item(i,0)->setFont(sansFont);
    }
    tableWidget->setVerticalHeaderLabels(nullist);
    long double pdfStdArray [n+1];
    for (int i=0;i<=n;++i){
        pdfStdArray[i]=binompdf(n,p,i);
    }
    int lower=round(mu-5*sigma);
    int higher=round(mu+5*sigma);

    int startstuffing=1;
    if(tSt1){
    for (int i=0; i<=n; ++i) {//binompdf col1
        if(i<lower or i>higher){
            tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(QString::number(0))));
            if(tSSt)col1.append(QString::number(0));
            tableWidget->item(i,startstuffing)->setFont(sansFont);
            continue;
        }
        QString tmp = QString::number(pdfStdArray[i],'f',5);
        if(tmp=="0.00000") tmp=QString::number(0);
        tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(tmp)));
        if(tSSt)col1.append(tr("%1").arg(tmp));
        tableWidget->item(i,startstuffing)->setFont(sansFont);
    }
    startstuffing++;
    }
    if(tSt2){
    long double sum=0;
    for (int i=0; i<=n; ++i) {//binomcdf(0,k) col2
        sum+=pdfStdArray[i];
        if(i<lower){
            tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(0)));
            if(tSSt)col2.append(QString::number(0));
            tableWidget->item(i,startstuffing)->setFont(sansFont);
            continue;
        }
        if(i>higher){
            tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(1)));
            if(tSSt)col2.append(QString::number(1));
            tableWidget->item(i,startstuffing)->setFont(sansFont);
            continue;
        }
        QString tmp = QString::number(sum,'f',5);
        if(tmp=="0.00000") tmp=QString::number(0);
        if(tmp=="1.00000") tmp=QString::number(1);
        tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(tmp)));
        if(tSSt)col2.append(tr("%1").arg(tmp));
        tableWidget->item(i,startstuffing)->setFont(sansFont);
    }
    startstuffing++;
    }
    if(tSt3){
    long double sum=1;
    for (int i=0; i<=n; ++i) {//binomcdf(k,n) col3
        if(!i){sum=1;}else{sum-=pdfStdArray[i-1];}
        if(i<round(lower)){
            tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(1)));
            if(tSSt)col3.append(QString::number(1));
            tableWidget->item(i,startstuffing)->setFont(sansFont);
            continue;
        }
        if(i>round(higher)){
            tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(0)));
            if(tSSt)col3.append(QString::number(0));
            tableWidget->item(i,startstuffing)->setFont(sansFont);
            continue;
        }
        QString tmp = QString::number(sum,'f',5);
        if(tmp=="0.00000") tmp=QString::number(0);
        if(tmp=="1.00000") tmp=QString::number(1);
        tableWidget->setItem(i, startstuffing, new QTableWidgetItem(tr(" %1 ").arg(tmp)));
        if(tSSt)col3.append(tr("%1").arg(tmp));
        tableWidget->item(i,startstuffing)->setFont(sansFont);
    }
    startstuffing++;
    }

    //eckdaten
    tableWidget->setItem(0,startstuffing,new QTableWidgetItem(tr(" n=%1 ").arg(n)));
    tableWidget->setItem(1,startstuffing,new QTableWidgetItem(tr(" p=%1 ").arg(p)));
    tableWidget->setItem(2,startstuffing,new QTableWidgetItem(tr(" μ=%1 ").arg(mu)));
    tableWidget->setItem(3,startstuffing,new QTableWidgetItem(tr(" σ=%1 ").arg(sigma)));
    col4.append(tr(" n=%1 ").arg(n));
    col4.append(tr(" p=%1 ").arg(p));
    col4.append(tr(" μ=%1 ").arg(mu));
    col4.append(tr(" σ=%1 ").arg(sigma));
    for(int i=0;i<=3;i++){
        tableWidget->item(i,startstuffing)->setFont(sansFont);
    }

    int c=ui->tabWidget->count();
    ui->tabWidget->insertTab(c,tableWidget,QString(">>Tabelle No.%1").arg(++tablecount));
    ui->tabWidget->setCurrentIndex(c);

    QString docPath=getDocPath();

    if(tSSt){
        QDir dir(QString("%1/Sitzung am %2").arg(docPath).arg(sessionDocname));
    if (!dir.exists()) dir.mkpath(".");

    QString dir_name(QString("%1/Sitzung am %2/tabelle no.%3").arg(docPath).arg(sessionDocname).arg(tablecount));
    QFile data(dir_name+".csv");
    if (data.open(QFile::WriteOnly | QIODevice::Append)){}
    QTextStream output(&data);
    //output <<"n"<< ";"<<"P(X=k)"<< ";"<<"P(X<=k)"<< ";"<<"P(X>=k)"<<";"<<"Eckdaten"<<'\n';
    output<<"n"<<";";
    if(tSt1)output<<"P(X=k)"<< ";";
    if(tSt2)output<<"P(X<=k)"<<";";
    if(tSt3)output<<"P(X>=k)"<<";";
    output<<"Eckdaten"<<'\n';
    for(int i=0;i<=n; i++) {
        //output  <<col0[i]<< ";" << (tSt1?col1[i]:"") <<";" << (tSt2?col2[i]:"") <<";" << (tSt3?col3[i]:"") <<";"<< (i<4?col4[i]:" ") << '\n';
        output<<col0[i]<<";";
        if(tSt1)output<<col1[i]<<";";
        if(tSt2)output<<col2[i]<<";";
        if(tSt3)output<<col3[i]<<";";
        output<<(i<4?col4[i]:" ")<<'\n';
    }

    data.close();

    }
}
////-----------end of block-----------

////  ---  <<< Wichtige Funktionen WF-Block >>> ---

void MainWindow::wfOutput(int id){
    switch (id){
        case 1://wf_pow
        {
        powtmp->res=1;
        wf_pow(powtmp);
        if(powtmp->shitsPivoted==1){
            ui->wf_factorial_out->setText("Ungültige Eingabe");
            powtmp->shitsPivoted=0;
            powtmp->overflow=0;
        }else if(powtmp->overflow==0&&powtmp->k>=0){
            ui->wf_factorial_out->setText(QString("%1^%2 = %3").arg(powtmp->n).arg(powtmp->k).arg(QString::number(powtmp->res,'f',0)));
        }else if(powtmp->overflow==0&&powtmp->k<0){
            ui->wf_factorial_out->setText(QString("%1^%2 = %3").arg(powtmp->n).arg(powtmp->k).arg(QString::number(powtmp->res,'f',10)));
        }else{
            ui->wf_factorial_out->setText(QString("%1^%2 = %3   E%4").arg(powtmp->n).arg(powtmp->negative?-(powtmp->k):powtmp->k).arg(QString::number(powtmp->res,'f',7)).arg(powtmp->overflow));
            powtmp->overflow=powtmp->negative=0;
        }

        }break;

        case 2://wf_npr
        {
        wf_npr(nprtmp);
        if(nprtmp->shitsPivoted==1){
            ui->wf_factorial_out->setText("Ungültige Eingabe");
            nprtmp->shitsPivoted=0;
        }else if(nprtmp->overflow==0){
            ui->wf_factorial_out->setText(QString("%1 P %2 = %3").arg(nprtmp->n).arg(nprtmp->k).arg(QString::number(nprtmp->res,'f',0)));
        }else{
            QString tmp=QString::number(nprtmp->res,'f',5);
            ui->wf_factorial_out->setText(QString("%1 P %2 = %3   E%4").arg(nprtmp->n).arg(nprtmp->k).arg(tmp=="inf"?"∞":tmp).arg(tmp=="inf"?0:nprtmp->overflow));
            nprtmp->overflow=0;

        }//!!!!!! write inf stuff better !!!!!!

        }break;

        case 3://wf_ncr
        {
        wf_ncr(ncrtmp);
        if(ncrtmp->shitsPivoted==1){
            ui->wf_factorial_out->setText("Ungültige Eingabe");
            ncrtmp->shitsPivoted=0;
        }else if(ncrtmp->overflow==0){
            ui->wf_factorial_out->setText(QString("%1 C %2 = %3").arg(ncrtmp->n).arg(ncrtmp->k).arg(QString::number(ncrtmp->res,'f',0)));
        }else{
            QString tmp=QString::number(ncrtmp->res,'f',5);
            ui->wf_factorial_out->setText(QString("%1 C %2 = %3   E%4").arg(ncrtmp->n).arg(ncrtmp->k).arg(tmp=="inf"?"∞":tmp).arg(tmp=="inf"?0:ncrtmp->overflow));
            ncrtmp->overflow=0;
        }

        }break;
    }
}
void MainWindow::on_wf_factorial_edit_textChanged(const QString &arg1)
{
    wf* tmp= new wf;
    tmp->n=arg1.toDouble();
    if(!tmp->n && arg1!="0") tmp->shitsPivoted=1;
    wf_factorial(tmp);
    if(tmp->shitsPivoted==1){
        ui->wf_factorial_out->setText("Ungültige Eingabe");
        tmp->shitsPivoted=0;
    }else if(tmp->overflow==0){
        ui->wf_factorial_out->setText(QString("%1! = %2").arg(tmp->n).arg(QString::number(tmp->res,'f',0)));
    }else{
        ui->wf_factorial_out->setText(QString("%1! = %2   E%3").arg(tmp->n).arg(QString::number(tmp->res,'f',5)).arg(tmp->overflow));
        tmp->overflow=0;
    }
}


void MainWindow::on_wf_n_pow_edit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->wf_n_pow_edit->setText(arg);
        ui->wf_k_pow_edit->setFocus();
    }
    powtmp->n=arg.toDouble();
    if(!powtmp->n && arg!="0") powtmp->shitsPivoted=1;
    wfOutput(1);
}


void MainWindow::on_wf_k_pow_edit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->wf_k_pow_edit->setText(arg);
        ui->wf_n_pow_edit->setFocus();
    }
    powtmp->k=arg.toDouble();
    if(!powtmp->k && arg!="0") powtmp->shitsPivoted=1;
    wfOutput(1);
}


void MainWindow::on_wf_n_npr_edit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->wf_n_npr_edit->setText(arg);
        ui->wf_r_npr_edit->setFocus();
    }
    nprtmp->n=arg.toDouble();
    if(!nprtmp->n && arg!="0") nprtmp->shitsPivoted=1;
    wfOutput(2);
}


void MainWindow::on_wf_r_npr_edit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->wf_r_npr_edit->setText(arg);
        ui->wf_n_npr_edit->setFocus();
    }
    nprtmp->k=arg.toDouble();
    if(!nprtmp->k && arg!="0") nprtmp->shitsPivoted=1;
    wfOutput(2);
}


void MainWindow::on_wf_n_ncr_edit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->wf_n_ncr_edit->setText(arg);
        ui->wf_r_ncr_edit->setFocus();
    }
    ncrtmp->n=arg.toDouble();
    if(!ncrtmp->n && arg!="0") ncrtmp->shitsPivoted=1;
    wfOutput(3);
}


void MainWindow::on_wf_r_ncr_edit_textChanged(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->wf_r_ncr_edit->setText(arg);
        ui->wf_n_ncr_edit->setFocus();
    }
    ncrtmp->k=arg.toDouble();
    if(!ncrtmp->k && arg!="0") ncrtmp->shitsPivoted=1;
    wfOutput(3);
}

////// ----------end of block--------------

////  ---  <<< Parameter Retrieval Kit PRK-Block >>> ---

void MainWindow::prkClearUp(){
    prkObj->resetprk();
    ui->prk_n_edit->clear();
    ui->prk_p_edit->clear();
    ui->prk_k1_edit->clear();
    ui->prk_k2_edit->clear();
    ui->prk_n_out->setText("n=?");
    ui->prk_p_out->setText("p=?");
    ui->prk_k1_out->setText("?");
    ui->prk_k2_out->setText("?");
    ui->prk_target_probability->clear();
}
void MainWindow::on_prk_targetParameter_currentIndexChanged(int index)
{
    if(ui->prk_k1_edit->isHidden())ui->prk_k1_edit->show();
    if(ui->prk_k2_edit->isHidden())ui->prk_k2_edit->show();
    if(ui->prk_n_edit->isHidden())ui->prk_n_edit->show();
    if(ui->prk_p_edit->isHidden())ui->prk_p_edit->show();

    if(ui->prk_k1_out->isHidden())ui->prk_k1_out->show();
    if(ui->prk_k2_out->isHidden())ui->prk_k2_out->show();
    if(ui->prk_n_out->isHidden())ui->prk_n_out->show();
    if(ui->prk_p_out->isHidden())ui->prk_p_out->show();

    prkClearUp();
    ui->prk_p_edit->setFocus();

    if(prk_cumulative){
        ui->prk_compStatement->setText("≤ X ≤");
    switch(index){
    case 0://n
    {
        ui->prk_n_edit->hide();
        ui->prk_p_out->hide();
        ui->prk_k1_out->hide();
        ui->prk_k2_out->hide();
        prk_missing=1;
        prk_circular_index=4;
        break;
    }
    case 1://p
    {
        ui->prk_p_edit->hide();
        ui->prk_n_out->hide();
        ui->prk_k1_out->hide();
        ui->prk_k2_out->hide();
        prk_missing=2;
        prk_circular_index=5;
        break;
    }
    case 2://k1
    {
        ui->prk_n_out->hide();
        ui->prk_p_out->hide();
        ui->prk_k1_edit->hide();
        ui->prk_k2_out->hide();
        prk_missing=4;
        prk_circular_index=6;
        break;
    }
    case 3://k2
    {
        ui->prk_n_out->hide();
        ui->prk_p_out->hide();
        ui->prk_k1_out->hide();
        ui->prk_k2_edit->hide();
        prk_missing=5;
        prk_circular_index=7;
        break;
    }
    }
    }else if(!prk_cumulative){//---------------------------------------------
            ui->prk_k1_out->hide();
            ui->prk_k1_edit->hide();
            ui->prk_compStatement->setText("X =");

    switch(index){
    case 0://n
    {
        ui->prk_n_edit->hide();
        ui->prk_p_out->hide();
        ui->prk_k2_out->hide();
        prk_missing=1;
        prk_circular_index=1;
        break;
    }
    case 1://p
    {
        ui->prk_p_edit->hide();
        ui->prk_n_out->hide();
        ui->prk_k2_out->hide();
        prk_missing=2;
        prk_circular_index=2;
        break;
    }
    case 2://k
    {
        ui->prk_n_out->hide();
        ui->prk_p_out->hide();
        ui->prk_k2_edit->hide();
        prk_missing=3;
        prk_circular_index=3;
        break;
    }
    }
}
}

void MainWindow::prkCirculate(int circle, QString callee){
    switch (circle){
///----------------------------prk_cumulative==0
    case 1://p->k2->er
    {
        if(callee=="p")ui->prk_k2_edit->setFocus();
        if(callee=="k2")ui->prk_target_probability->setFocus();
        if(callee=="er")ui->prk_p_edit->setFocus();
        break;
    }
    case 2://n->k2->er
    {
        if(callee=="n")ui->prk_k2_edit->setFocus();
        if(callee=="k2")ui->prk_target_probability->setFocus();
        if(callee=="er")ui->prk_n_edit->setFocus();
        break;
    }
    case 3://n->p->er
    {
        if(callee=="n")ui->prk_p_edit->setFocus();
        if(callee=="p")ui->prk_target_probability->setFocus();
        if(callee=="er")ui->prk_n_edit->setFocus();
        break;
    }
///----------------------------prk_cumulative==1
    case 4://p->k1->k2->er
    {
        if(callee=="p")ui->prk_k1_edit->setFocus();
        if(callee=="k1")ui->prk_k2_edit->setFocus();
        if(callee=="k2")ui->prk_target_probability->setFocus();
        if(callee=="er")ui->prk_p_edit->setFocus();
        break;
    }
    case 5://n->k1->k2->er
    {
        if(callee=="n")ui->prk_k1_edit->setFocus();
        if(callee=="k1")ui->prk_k2_edit->setFocus();
        if(callee=="k2")ui->prk_target_probability->setFocus();
        if(callee=="er")ui->prk_n_edit->setFocus();
        break;
    }
    case 6://n->p->k2->er
    {
        if(callee=="n")ui->prk_p_edit->setFocus();
        if(callee=="p")ui->prk_k2_edit->setFocus();
        if(callee=="k2")ui->prk_target_probability->setFocus();
        if(callee=="er")ui->prk_n_edit->setFocus();
        break;
    }
    case 7://n->p->k1->er
    {
        if(callee=="n")ui->prk_p_edit->setFocus();
        if(callee=="p")ui->prk_k1_edit->setFocus();
        if(callee=="k1")ui->prk_target_probability->setFocus();
        if(callee=="er")ui->prk_n_edit->setFocus();
        break;
    }
    }
}

void MainWindow::on_prk_cumulativeP_clicked(bool checked)
{
    prkClearUp();
    if(checked){
        prk_cumulative=1;
        ui->prk_targetParameter->clear();
        ui->prk_targetParameter->addItems({"Stichprobenumfang n","Trefferquote p","Untere Grenze k1","Obere Grenze k2"});

    }else{
        prk_cumulative=0;
        ui->prk_targetParameter->clear();
        ui->prk_targetParameter->addItems({"Stichprobenumfang n","Trefferquote p","Ereignis k"});
    }
}

void MainWindow::on_prk_show_other_limit_clicked(bool checked)
{
    if(checked){prk_show_other_limit++;}else{prk_show_other_limit=0;}
    prkOutput(prk_missing);
}

void MainWindow::on_prk_n_edit_textEdited(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->prk_n_edit->setText(arg);
        prkCirculate(prk_circular_index,"n");
    }
    prkObj->n=arg.toInt()>=0?arg.toInt():-1;
    if(!prkObj->n && arg!="0") prk_cought_fire=1;
    prkOutput(prk_missing);
}


void MainWindow::on_prk_p_edit_textEdited(const QString &arg1)
{
    QString arg=specialDoubleInput(arg1);
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->prk_p_edit->setText(arg);
        prkCirculate(prk_circular_index,"p");
    }
    prkObj->p=arg.toDouble()>=0&&arg.toDouble()<1?arg.toDouble():-1;
    if(!prkObj->p && arg!="0") prk_cought_fire=1;
    prkOutput(prk_missing);
}


void MainWindow::on_prk_k1_edit_textEdited(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->prk_k1_edit->setText(arg);
        prkCirculate(prk_circular_index,"k1");
    }
    prkObj->k1=arg.toInt()>=0?arg.toInt():-1;
    if(!prkObj->k1 && arg!="0") prk_cought_fire=1;
    prkOutput(prk_missing);
}


void MainWindow::on_prk_k2_edit_textEdited(const QString &arg1)
{
    QString arg=arg1;
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->prk_k2_edit->setText(arg);
        prkCirculate(prk_circular_index,"k2");
    }
    if(!prk_cumulative){
        prkObj->k=arg.toInt()>=0?arg.toInt():-1;
        if(!prkObj->k && arg!="0") prk_cought_fire=1;
    }

    QMessageBox mes;
    mes.setWindowTitle("Warnung");
    mes.setText("Für Eingaben von k1>2000 und/oder p<0.5 könnte die Rechnung erheblich verzögert werden und das Programm könnte eventuell abstürzen!");

    if(prk_cumulative){
        if(arg[0]=='n'){
            delay(2);
            if(!prkObj->warnung)mes.exec();
            prkObj->warnung++;
            arg="1010101010";
        }else{
            prkObj->warnung=0;
        }
        prkObj->k2=arg.toInt()>=0?arg.toInt():-1;
        if(!prkObj->k2 && arg!="0") prk_cought_fire=1;
    }
    prkOutput(prk_missing);
}


void MainWindow::on_prk_target_probability_textEdited(const QString &arg1)
{
    QString arg=specialDoubleInput(arg1);
    if(arg!=NULL && arg[arg.length()-1]==(char)32){
        arg.remove(' ');
        ui->prk_target_probability->setText(arg);
        prkCirculate(prk_circular_index,"er");
    }
    prkObj->pRes=arg.toDouble()>0&&arg.toDouble()<1?arg.toDouble():-1;
    if(!prkObj->pRes && arg!="0") prk_cought_fire=1;
    prkOutput(prk_missing);
}

void MainWindow::on_prk_2nd_compStatement_currentIndexChanged(int index)
{
    prkObj->cmpstatus=index;
    prkOutput(prk_missing);
}

void MainWindow::on_prk_goToStd_clicked()
{
    if(prkObj->n==-1 or prkObj->p==-1 or prkObj->k==-1 or prkObj->k1==-1 or prkObj->k2==-1 or prkObj->pRes==-1 or prkObj->n==0 or prkObj->p==0 or prkObj->n<prkObj->k or prkObj->n<prkObj->k2 or prkObj->k2<prkObj->k1 ){
        QMessageBox mes;
        mes.setWindowTitle("Warnung");
        mes.setText("Ungültige Werte können nicht kopiert werden!");
        mes.exec();
        return;
    }
    if(setToParam==2){
        ui->calWithParam->setCheckState(Qt::Unchecked);
        on_calWithParam_clicked(0);
    }
    if(!prk_cumulative){
        ui->cumulativeP->setCheckState(Qt::Unchecked);
        on_cumulativeP_clicked(0);
        n=prkObj->n;    ui->_n_edit->setText(QString::number(n));
        p=prkObj->p;    ui->_p_edit->setText(QString::number(p));
        k1=prkObj->k;   ui->underEdit->setText(QString::number(k1));
    }else{
        ui->cumulativeP->setCheckState(Qt::Checked);
        on_cumulativeP_clicked(1);
        n=prkObj->n;    ui->_n_edit->setText(QString::number(n));
        p=prkObj->p;    ui->_p_edit->setText(QString::number(p));
        k1=prkObj->k1;   ui->underEdit->setText(QString::number(k1));
        k2=prkObj->k2==1010101010?prkObj->n:prkObj->k2;   ui->upperEdit->setText(QString::number(k2));
    }
    ui->tabWidget->setTabVisible(1,1);
    ui->tabWidget->setCurrentIndex(1);
}
QString formatOtherLimit(int cumulative, char missing, double firstLimit, int prk_show_other_limit){
    double res=prkObj->getOtherLimit(cumulative,missing,firstLimit);
    if(res==-1 or res<firstLimit or !prk_show_other_limit) return "";
    return QString("%1≥").arg(res);
}
void MainWindow::prkOutput(int missing){//missing: n<=>1 p<=>2 k<=>3 k1<=>4 k2<=>5
    switch (missing){
        case 1:{
        if(prkObj->p==0){ ui->prk_n_out->setText("n=? (Ungültige Eingabe)"); break;}
            int tmp=prkObj->nMissing(prk_cumulative);
            QString tmp2=formatOtherLimit(prk_cumulative,'n',tmp,prk_show_other_limit);
            QString cmp=prkObj->cmpstatus?"≤":"≥";
            if(prk_cought_fire)tmp=-1;
            qDebug()<<"ntmp:"<<tmp;
            if(tmp!=-1){ui->prk_n_out->setText(QString("%3n%1%2").arg(cmp).arg(tmp).arg(tmp2));}else{ui->prk_n_out->setText("n=? (Ungültige Eingabe)");prk_cought_fire++;}
            break;
        }//"≥","≤"
        case 2:{
            double tmp=prkObj->pMissing(prk_cumulative);
            QString tmp2=formatOtherLimit(prk_cumulative,'p',tmp,prk_show_other_limit);
            QString cmp=prkObj->cmpstatus?(prk_cumulative?(prkObj->k1?"≤":"≥"):(prkObj->k?"≤":"≥")):(prk_cumulative?(prkObj->k1?"≥":"≤"):(prkObj->k?"≥":"≤"));
            if(prk_cought_fire)tmp=-1;
            qDebug()<<"ptmp:"<<tmp;
            if(tmp!=-1){ui->prk_p_out->setText(QString("%3p%1%2").arg(cmp).arg(tmp).arg(tmp2));}else{ui->prk_p_out->setText("p=? (Ungültige Eingabe)");prk_cought_fire++;}
            break;
        }
        case 3:{
            int tmp=prkObj->kMissing(prk_cumulative);
            if(prk_cought_fire)tmp=-1;
            qDebug()<<"ktmp:"<<tmp;
            if(tmp!=-1){ui->prk_k2_out->setText(QString("%1").arg(tmp));}else{ui->prk_k2_out->setText(" ? (Ungültige Eingabe)");prk_cought_fire++;}
            break;
        }
        case 4:{
            int tmp=prkObj->k1Missing(prk_cumulative);
            if(prk_cought_fire)tmp=-1;
            qDebug()<<"k1tmp:"<<tmp;
            if(tmp!=-1){ui->prk_k1_out->setText(QString("%1").arg(tmp));}else{ui->prk_k1_out->setText(" ? (Ungültige Eingabe)");prk_cought_fire++;}
            break;
        }
        case 5:{
            int tmp=prkObj->k2Missing(prk_cumulative);
            if(prk_cought_fire)tmp=-1;
            qDebug()<<"k2tmp:"<<tmp;
            if(tmp!=-1){ui->prk_k2_out->setText(QString("%1").arg(tmp));}else{ui->prk_k2_out->setText(" ? (Ungültige Eingabe)");prk_cought_fire++;}
            break;
        }
    }

    double localMu=0,localSigma=0;
    if(fromNandP(prkObj->n,prkObj->p,localMu,localSigma)==-1)
    {
        ui->prk_mu_sigma->setText("(μ=?      σ=?)(Ungültige Eingabe)");
    }else{

        ui->prk_mu_sigma->setText(QString("(μ=%1      σ=%2)").arg(localMu).arg(localSigma));
    }
        prk_cought_fire=0;
}

////// ----------end of block--------------

///// ------<<<<<Lucky Block>>>>>-------

void MainWindow::on_lucky_button_clicked()
{
    quint32 rando1=QRandomGenerator::global()->bounded(8);
    while(rando1==3)rando1=QRandomGenerator::global()->bounded(8);
    signal=0;
    easteregg==2?easteregg++:easteregg=0;
    switch (rando1){
    case 1:
    {
            this->resize(1480,870);
            this->move(200,50);
            ui->textBrowserX->setHtml(pi);
            break;
    }
    case 2:
    {
            this->resize(1480,980);
            this->move(200,50);
            ui->textBrowserX->setHtml(bridge);
            break;
    }
    case 3:
    {/*
            this->resize(900,535);
            this->move(300,200);
            //ui->textBrowserX->setFont(QFont("Monaco"));
            //ui->textBrowserX->setText(lucky2);
            ui->lucky_button->setText(lucky);
            luckyLoki();
    */
            break;
    }
    case 4:
    {
            this->resize(1234,750);
            this->move(300,90);
            ui->textBrowserX->setHtml(fiat);
            break;
    }
    case 5:
    {
            this->resize(1234,870);
            this->move(300,50);
            ui->textBrowserX->setHtml(jasmine);
            break;
    }
    case 6:
    {
            this->resize(1234,970);
            this->move(300,50);
            ui->textBrowserX->setHtml(beach);
            break;
    }

    default:
    {
            this->resize(1480,980);
            this->move(200,50);
            ui->textBrowserX->setHtml(bridge);
            break;
    }
    }
}



void MainWindow::changeSizeEvent(){ //show irrational numbers on welcome screen
    int rando=QRandomGenerator::global()->bounded(200);
    QString chosenIrrational;
    if(!(rando%3)){chosenIrrational=piTo1000;}else if((rando%3)==1){chosenIrrational=eTo330;}else{chosenIrrational=sqrt2ToN;}
    int counter=320;
    QString pre, arg, post(R"(</span>)");
    signal=0;signal++;
    running++;
    ui->textBrowserX->setToolTip("Welche irrationale Zahl ist das?");
    while(counter&&signal&&running){
            pre = QString(R"(<span style="font-size: 86px; font-family: Broadway; color: rgb(%1, %2, %3);">)").arg(QRandomGenerator::global()->bounded(255)).arg(QRandomGenerator::global()->bounded(255)).arg(QRandomGenerator::global()->bounded(255));
            arg.push_back(QString("%1%2%3").arg(pre).arg(chosenIrrational.at(320-counter)).arg(post));
            ui->textBrowserX->setHtml(arg);
            delay(360);
            while(ui->tabWidget->currentIndex()!=0){
                delay(500);
            }
            counter--;
    }
    ui->textBrowserX->setToolTip(";)");
    running=0;
}

void MainWindow::firstWelcome(){
    QString space;
    QString space2;
    for(int i=0;i<59;i++){
                space.append("&nbsp;");
        if(i<16)space2.append("&nbsp;");
    }
    ui->textBrowserX->setHtml(QString(R"(<br><br><br><br><br><br>%1<span style="font-size: 100px;font-family: Agency FB;"> Willkommen zum </span>)").arg(space));
    delay(1111);
    for(int i=20;i<=255;i=i+5){
        ui->textBrowserX->setHtml("<br>"+QString(R"(%2<span style="font-size: 200px; color:rgb(%1, %1, %1); font-family: Agency FB;">🂠 </span>)").arg(i).arg(space2)+QString(R"(<span style="font-size: 150px; color:rgb(%1, %1, %1); font-family: Agency FB;">Binomialrechner</span>)").arg(i));
        delay(42);
    }
}

void MainWindow::welcomeDisplay(){
    /*
        int i=25;
        signal++;
        while(i&&signal){

            ui->textBrowserX->setHtml(welcome1);
            delay(3000);
            ui->textBrowserX->setHtml(welcome2);
            delay(3000);
            i--;
        }
*/
    QString space;
    for(int i=0;i<16;i++){
            space.append("&nbsp;");

    }
    QString dices="⚀⚁⚂⚃⚄⚅";
    QString cards[]={"🂠","🂡","🂢","🂣","🂤","🂥","🂦","🂧","🂨","🂩","🂪","🂫","🂬","🂭","🂮","🂿"};
    QString moonphases[]={"🌕","🌖","🌗","🌘","🌑","🌒","🌓","🌔"};
    int i=0,j=0;
    signal=0;signal++;
    welcomeRunning++;
    while(signal&&i!=300){
            //ui->textBrowserX->setFont(QFont("Agency FB",130));
            //ui->textBrowserX->setHtml("&nbsp;&nbsp;&nbsp;"+cards[j]+QString(R"(<span style="font-size: 150px;"> Binomialrechner</span>)"));
            ui->textBrowserX->setHtml("<br>"+QString(R"(%2<span style="font-size: 200px; font-family: Agency FB;">%1 </span>)").arg(cards[j]).arg(space)+QString(R"(<span style="font-size: 150px; font-family: Agency FB;">Binomialrechner</span>)"));
            delay(1800);
            while(ui->tabWidget->currentIndex()!=0){
                delay(500);
            }
            i++;j++;
            if(j==16)j=0;
    }
    welcomeRunning=0;

}

void MainWindow::luckyLoki(){
    QString lucky="I'm feeling lucky!😂";
    ui->textBrowserX->setHtml(QString(R"(&nbsp;&nbsp;&nbsp;&nbsp;<span style="font-size: 120px; font-family: Mistral; color: rgb(255,255,255);">%2</span>)").arg(lucky));

    /*
    int i=32,rando;
    QFontDatabase f;
    QStringList fonts=f.families();

    while(i){
        rando=QRandomGenerator::global()->bounded(370);
        ui->textBrowserX->setHtml(QString(R"(<span style="font-size: 120px; font-family: %1; color: rgb(255,255,255);">%2</span>)").arg(fonts.at(rando)).arg(lucky));
        delay(2024);
        i--;
    }
*/
}

////// ----------end of block--------------
