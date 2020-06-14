#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "weatherdatahistory.h"

#include <QMainWindow>
#include <QDebug>
#include <QList>
#include <QSerialPort>
#include <QDateTime>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

// Obsluga plikow
#include <QFile>
#include <QTextStream>

// Obsluga zegara
#include <QTimer>

// Obsluga wykresow
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>

// Obsluga historii pogody
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateCharts(unsigned long long t_time);

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pushButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_SettingsTabSearchPushButton_clicked();

    void on_SettingsTabConnectPushButton_clicked();

    void on_SettingsTabDisconnectPushButton_clicked();

    void readFromPort();


    void clockTimerFctn();


private:
    QSerialPort *device;
    Ui::MainWindow *ui;
    QTimer *clockTimer;
    QTime chartTime;


    void addToLogs(QString message);
    void initConnectionInformation();
    void displayConnectionInformation();
    void setConnectionStatusImage(bool t_connectionStatus);

    void readFromFile();

    void setVariablesFromFileLine();

    void setWeatherIcon(int t_insolation, int t_rainfall);
    void setDetailWeatherValues();

    void showPastTemperature(weatherDataHistory t_pastData);

    // Connection informations
    QString connectionStatus;
    QString baudrate;
    QString dataBits;
    QString parity;
    QString stopBits;
    QString flowControl;

    // File to save data

    // Variables for last data
    QString currentRawDataStr;

    QString fullDateStr;
    QString dateStr;
    QString hourStr;

    QString startBitStr; // $
    QString temperatureStr;
    QString humidityStr;
    QString pressureStr;
    QString rainfallStr;
    QString insolationStr;
    QString stopBit; // X\r\n

    double currentTemperature = 0.0;
    double currentHumidity = 0.0;
    double currentPressure = 0.0;
    double currentRainfall = 0.0;
    double currentInsolation = 0.0;

    // WYKRESY

    // Wyswietlenie
    QChartView* temperatureChartView;
    QChartView* humidityChartView;
    QChartView* pressureChartView;
    QChartView* insolationChartView;

    // Serie
    QLineSeries* temperatureChartSeries;
    QLineSeries* humidityChartSeries;
    QLineSeries* pressureChartSeries;
    QLineSeries* insolationChartSeries;

    // Osie czasu
    QValueAxis* temperatureTimelineAxis;
    QValueAxis* humidityTimelineAxis;
    QValueAxis* pressureTimelineAxis;
    QValueAxis* insolationTimelineAxis;

    // Osie wartosci
    QValueAxis* temperatureValueAxis;
    QValueAxis* humidityValueAxis;
    QValueAxis* pressureValueAxis;
    QValueAxis* insolationValueAxis;

    // Wykresy
    QChart* temperatureChart;
    QChart* humidityChart;
    QChart* pressureChart;
    QChart* insolationChart;

    void resizeEvent(QResizeEvent* event);


    // WYKRESY - FUNKCJE //
    void createCharts();

    void createSeries();
    void createAxes();
    void makeChart();
    void attachSeries();
    void showChart();

    void initWeatherHistory();


};
#endif // MAINWINDOW_H
