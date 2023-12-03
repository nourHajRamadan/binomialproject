#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mathstuff.cpp"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

//Palette Settings
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QColor darkGray(53, 53, 53);
    QColor gray(128, 128, 128);
    QColor black(25, 25, 25);
    QColor blue(145, 19, 209);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, darkGray);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, black);
    darkPalette.setColor(QPalette::AlternateBase, darkGray);
    darkPalette.setColor(QPalette::ToolTipBase, blue);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, darkGray);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Link, blue);
    darkPalette.setColor(QPalette::Highlight, blue);
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



void MainWindow::on_calWithParam_clicked(bool checked)
{
    if (checked){//mit Mu und Sigma rechnen
        ui->_n_label->hide();
        ui->_p_label->hide();
        ui->_n_edit->hide();
        ui->_p_edit->hide();

        ui->_mu_label->show();
        ui->_sigma_label->show();
        ui->_m_edit->show();
        ui->_sigma_edit->show();

        setToParam=2;


        binomOutput(setToCumulative,setToParam);



    }else{//mit N und P rechnen
        ui->_n_label->show();
        ui->_p_label->show();
        ui->_n_edit->show();
        ui->_p_edit->show();

        ui->_mu_label->hide();
        ui->_sigma_label->hide();
        ui->_m_edit->hide();
        ui->_sigma_edit->hide();

        setToParam=1;

        binomOutput(setToCumulative,setToParam);
    }
}

//j=1 --> binompdf  j=2 --> binomcdf
//y=1 --> mit N und P rechnen y=2 --> mit Mu und Sigma rechnen
void MainWindow::binomOutput(int j, int y){
    //ui->hiddenOutput->setText(" ");

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
    }else{
            QString res =QString::number(binompdf(n,p,k1),'f',10);
            ui->pOutput->setText(QString("P(X=%1)=%2").arg(k1).arg(res));
        }
    }
    if(j==2){
        if(binomcdf(n,p,k1,k2)==-1){
            sthsWrong();
    }else{
            QString res =QString::number(binomcdf(n,p,k1,k2),'f',10);
            if(!k1){//höchstens -- at most
                ui->pOutput->setText(QString("P(X≤%1)=%2").arg(k2).arg(res));
            }
            else if(k2==n){//mindestens -- at least
                ui->pOutput->setText(QString("P(X≥%1)=%2").arg(k1).arg(res));
            }else if(k1==k2){//identische Grenzen -- identical limits
                ui->pOutput->setText(QString("P(X=%1)=%2").arg(k1).arg(res));
            }else{//beidseitig -- two different limits
                ui->pOutput->setText(QString("P(%1≤X≤%2)=%3").arg(k1).arg(k2).arg(res));
            }

        }
    }

}

void MainWindow::sthsWrong(){

        ui->hiddenOutput->setText("Ungültige/unvollständige Eingabe!");
}

void MainWindow::on__n_edit_textChanged(const QString &arg1)
{
    n=arg1.toInt()>=0?arg1.toInt():0;
    qDebug()<<"n:"<<n;
    binomOutput(setToCumulative,setToParam);

}


void MainWindow::on__p_edit_textChanged(const QString &arg1)
{
    p=arg1.toDouble()>=0?arg1.toDouble():0;
    qDebug()<<"p:"<<p;
    binomOutput(setToCumulative,setToParam);
}


void MainWindow::on_underEdit_textChanged(const QString &arg1)
{
    k1=arg1.toInt()>=0?arg1.toInt():0;
    qDebug()<<"k1:"<<k1;
    binomOutput(setToCumulative,setToParam);
}


void MainWindow::on_upperEdit_textChanged(const QString &arg1)
{
    k2=arg1.toInt()>=0?arg1.toInt():0;
    qDebug()<<"k2:"<<k2;
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
        binomOutput(setToCumulative,setToParam);
    }
    if(checked){//binomcdf
        ui->upperEdit->show();
        ui->upperLabel->show();
        ui->underLabel->setText("untere Grenze");
        ui->underEdit->setPlaceholderText(" ");
        setToCumulative=2;
        binomOutput(setToCumulative,setToParam);
    }
}


void MainWindow::on__m_edit_textChanged(const QString &arg1)
{
    mu=arg1.toDouble()>=0?arg1.toDouble():0;
    qDebug()<<"mu:"<<mu;
    binomOutput(setToCumulative,setToParam);
}


void MainWindow::on__sigma_edit_textChanged(const QString &arg1)
{
    sigma=arg1.toDouble()>=0?arg1.toDouble():0;
    qDebug()<<"sigma:"<<sigma;
    binomOutput(setToCumulative,setToParam);
}

/*  ---not figured out yet---
void MainWindow::on_showHisto_clicked()
{
    QBarSet* disto = new QBarSet("n");
    for (int i = 0; i <= n; ++i) {
        *disto<<binompdf(n,p,i);
    }
    QBarSeries *series = new QBarSeries();
    series->append(disto);
    QChart *chart = new QChart();

    chart->addSeries(series);

    chart->setTitle("Histogramm");

    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);



    grScene.addWidget(chartView);
    ui->graphicsView->setScene(&grScene);

}
*/
