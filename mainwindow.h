#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QLocale>
#include <QDebug>
#include <QColor>
#include <QPalette>
#include <QStyleFactory>
#include <QDialog>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidget>
#include <QFont>
#include <QTextBrowser>
#include <QRandomGenerator>

#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QTimer>

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QBarSet>
#include <QStackedBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>

#include <QPixmap>
#include <QOpenGLWidget>
#include <QPainter>
#include <QDesktopServices>
#include <QDir>
#include <QDateTime>
#include <QKeyEvent>
#include <QFontDatabase>

#define STDW 1145 //Standard Window Size
#define STDH 606

QT_CHARTS_USE_NAMESPACE
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void binomOutput(int j, int y);
    void sthsWrong();
    void wfOutput(int id);
    void prkOutput(int missing);
    void prkClearUp();
    void prkCirculate(int circle, QString callee);
    void firstWelcome();
    void welcomeDisplay();
    void resizeEvent(QResizeEvent* event);
    void changeSizeEvent();
    void luckyLoki();
    void fetchUpdate(); void offerUpdate();
    int signal=0,running=0;//for changeSizeEvent
    int welcomeRunning=0;
    int sysXmax = GetSystemMetrics(SM_CXSCREEN);
    int sysYmax = GetSystemMetrics(SM_CYSCREEN);
private slots:

    void on_calWithParam_clicked(bool checked);

    void on__n_edit_textChanged(const QString &arg1);

    void on__p_edit_textChanged(const QString &arg1);

    void on_underEdit_textChanged(const QString &arg1);

    void on_upperEdit_textChanged(const QString &arg1);

    void on_cumulativeP_clicked(bool checked);

    void on__m_edit_textChanged(const QString &arg1);

    void on__sigma_edit_textChanged(const QString &arg1);

    void on_wf_factorial_edit_textChanged(const QString &arg1);

    void on_wf_n_pow_edit_textChanged(const QString &arg1);

    void on_wf_k_pow_edit_textChanged(const QString &arg1);

    void on_wf_n_npr_edit_textChanged(const QString &arg1);

    void on_wf_r_npr_edit_textChanged(const QString &arg1);

    void on_wf_n_ncr_edit_textChanged(const QString &arg1);

    void on_wf_r_ncr_edit_textChanged(const QString &arg1);

    void on_showHisto_clicked();

    void on_tabWidget_tabCloseRequested(int index);

    void on_show_std_button_clicked();

    void on_show_wf_button_clicked();

    void on_show_prk_button_clicked();

    void on_showTable_clicked();

    void on_lucky_button_clicked();

    void on_show_manuel_button_clicked();

    void on_tSt1_clicked(bool checked);

    void on_tSt2_clicked(bool checked);

    void on_tSt3_clicked(bool checked);

    void on_savePNG_clicked(bool checked);

    void on_showPNGTarget_clicked();

    void on_saveCSV_clicked(bool checked);

    void on_prk_targetParameter_currentIndexChanged(int index);

    void on_prk_cumulativeP_clicked(bool checked);

    void on_prk_n_edit_textEdited(const QString &arg1);

    void on_prk_p_edit_textEdited(const QString &arg1);

    void on_prk_k1_edit_textEdited(const QString &arg1);

    void on_prk_k2_edit_textEdited(const QString &arg1);

    void on_prk_target_probability_textEdited(const QString &arg1);

    void on_instructionsTo_currentIndexChanged(int index);

    void on_prk_2nd_compStatement_currentIndexChanged(int index);

    void on_tabWidget_tabBarClicked(int index);

    void on_prk_goToStd_clicked();

    void on_std_choose_marked_interval_currentIndexChanged(int index);

    void on_std_show_options_clicked(bool checked);

    void on_std_show_histo_markings_clicked(bool checked);


private:
    Ui::MainWindow *ui;
    int currentSTDW=STDW,currentSTDH=STDH;
    int easteregg=0;
    int firstsignal=0;

    int n=0,k1=0,k2=0;
    double p=0,mu=0,sigma=0;
    int setToCumulative=1;
    int setToParam=1;
    int shitstorm=0;

    int histocount=0, tablecount=0;
    int tSt1=1; int tSt2=1; int tSt3=1; int tSSt=0; //Table showing and saving status
    int hSt=0;   unsigned int tipSt=2;           //Histogramm saving status
    quint32 histo_png_rando=QRandomGenerator::global()->bounded(1000000,9999999);
    int histoAni=1;
    int histoMarkings=0;
    QDateTime q=q.currentDateTime();
    QString sitzung = q.toString();

    int prk_cumulative=0;
    int prk_missing=0;
    int prk_cought_fire=0;
    int prk_circular_index=0;

    QString updateinfo;
};
#endif // MAINWINDOW_H


/* <<<<<TO DO>>>>>   <<v2.1.0>> (6) <<v2.1.1>> (2)
 * better view after choosing coloumn for table <<v2.1.0>>
 * Update: add>>Histo red marking, overall check and update <<v2.1.0>>
 * fix n and p interval bug in prk <<v2.1.0>>
 * histo settings for confidence intervals <<v2.1.0>>
 * set up a messagebox to communicate a special message with users <<v2.1.0>>
 * random dice numbers for i'm feeling lucky <<v2.1.0>>
 * fix all monitor specific settings <<v2.1.1>>
 * search function for table <<v2.1.1>>
 * windows installer
   >Nullsoft Scriptable Install System (NSIS)
   >InstallSimple
 * settings tab (merge into instructions)
   >change Histo theme, hide title
   >change target directory
   >control sizing, maximizing
   >search for updates, don't show updates while starting
   >toggle language (when translation is available)
 * create play button for Jazz Sakura (QAudioengine) (optional)
 * create binomial tutorials with twine (optional)
 * fix feelingLoki (optional)
 * improve n^k for negative k (optional)
 * cumulative histogram with connective line
 * build real npr (optional)
 * make an english version OR refactor for any translation
 * make histogramm interactive with qbarset signals (optional)
 *
 * >>>>> build wasm app
*/

//windeployqt.exe .

