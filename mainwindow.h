#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QLocale>
#include <QDebug>
#include <QColor>
#include <QPalette>
#include <QStyleFactory>
#include <QDialog>


#include <QApplication>

#include <QtWidgets/QMainWindow>

#include <QtCharts/QChartView>

#include <QtCharts/QBarSeries>

#include <QtCharts/QBarSet>

#include <QtCharts/QLegend>

#include <QtCharts/QBarCategoryAxis>

#include <QtCharts/QHorizontalStackedBarSeries>

#include <QtCharts/QLineSeries>

#include <QtCharts/QCategoryAxis>


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


private slots:

    void on_calWithParam_clicked(bool checked);

    void on__n_edit_textChanged(const QString &arg1);

    void on__p_edit_textChanged(const QString &arg1);

    void on_underEdit_textChanged(const QString &arg1);

    void on_upperEdit_textChanged(const QString &arg1);

    void on_cumulativeP_clicked(bool checked);

    void on__m_edit_textChanged(const QString &arg1);

    void on__sigma_edit_textChanged(const QString &arg1);

    //void on_showHisto_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene grScene;

    int n=0,k1=0,k2=0;
    double p=0,mu=0,sigma=0;
    int setToCumulative=1;
    int setToParam=1;

};
#endif // MAINWINDOW_H
