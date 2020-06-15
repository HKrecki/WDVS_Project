/*!
 * \file
 * \brief Definicje funkcji obsługi wykresów
 *
 * Plik zawiera funkcje wyświtelania i obsługi wykesów.
 *
 */




#include "mainwindow.h"



void MainWindow::createSeries(){
    this->temperatureChartSeries = new QLineSeries();
    // this->temperatureChartSeries->append(0,0);

    this->humidityChartSeries = new QLineSeries();
    // this->humidityChartSeries->append(0,0);

    this->insolationChartSeries = new QLineSeries();
    // this->insolationChartSeries->append(0,0);

    this->pressureChartSeries = new QLineSeries();
    // this->pressureChartSeries->append(0,0);
}

void MainWindow::createAxes(){
    // Temperature
    this->temperatureTimelineAxis = new QValueAxis;
    this->temperatureTimelineAxis->setLabelFormat("%.2f");
    this->temperatureTimelineAxis->setRange(0,24);
    this->temperatureTimelineAxis->setTickCount(6);

    this->temperatureValueAxis = new QValueAxis;
    this->temperatureValueAxis->setLabelFormat("%.1f");
    this->temperatureValueAxis->setRange(-20,40);
    this->temperatureValueAxis->setTickCount(4);

    // Humidity
    this->humidityTimelineAxis = new QValueAxis;
    this->humidityTimelineAxis->setLabelFormat("%.2f");
    this->humidityTimelineAxis->setRange(0,24);
    this->humidityTimelineAxis->setTickCount(6);

    this->humidityValueAxis = new QValueAxis;
    this->humidityValueAxis->setLabelFormat("%.1f");
    this->humidityValueAxis->setRange(0, 100);
    this->humidityValueAxis->setTickCount(4);

    // Insolation
    this->insolationTimelineAxis = new QValueAxis;
    this->insolationTimelineAxis->setLabelFormat("%.1f");
    this->insolationTimelineAxis->setRange(0,24);
    this->insolationTimelineAxis->setTickCount(6);

    this->insolationValueAxis = new QValueAxis;
    this->insolationValueAxis->setLabelFormat("%.2f");
    this->insolationValueAxis->setRange(0,100);
    this->insolationValueAxis->setTickCount(4);

    // Pressure
    this->pressureTimelineAxis = new QValueAxis;
    this->pressureTimelineAxis->setLabelFormat("%.1f");
    this->pressureTimelineAxis->setRange(0, 24);
    this->pressureTimelineAxis->setTickCount(6);

    this->pressureValueAxis = new QValueAxis;
    this->pressureValueAxis->setLabelFormat("%.2f");
    this->pressureValueAxis->setRange(950,1080);
    this->pressureValueAxis->setTickCount(4);
}

void MainWindow::makeChart(){
    this->temperatureChart = new QChart();
    this->temperatureChart->addAxis(temperatureValueAxis,Qt::AlignLeft);
    this->temperatureChart->addAxis(this->temperatureTimelineAxis,Qt::AlignBottom);
    this->temperatureChart->addSeries(this->temperatureChartSeries);
    this->temperatureChart->setTitle("Temperature [°C]");
    this->temperatureChart->legend()->hide();

    this->humidityChart = new QChart();
    this->humidityChart->addAxis(humidityValueAxis, Qt::AlignLeft);
    this->humidityChart->addAxis(this->humidityTimelineAxis, Qt::AlignBottom);
    this->humidityChart->addSeries(this->humidityChartSeries);
    this->humidityChart->setTitle("Humidity [%]");
    this->humidityChart->legend()->hide();

    this->insolationChart = new QChart();
    this->insolationChart->addAxis(insolationValueAxis, Qt::AlignLeft);
    this->insolationChart->addAxis(this->insolationTimelineAxis, Qt::AlignBottom);
    this->insolationChart->addSeries(this->insolationChartSeries);
    this->insolationChart->setTitle("Insolation [%]");
    this->insolationChart->legend()->hide();

    this->pressureChart = new QChart();
    this->pressureChart->addAxis(pressureValueAxis, Qt::AlignLeft);
    this->pressureChart->addAxis(this->pressureTimelineAxis, Qt::AlignBottom);
    this->pressureChart->addSeries(this->pressureChartSeries);
    this->pressureChart->setTitle("Pressure [hPa]");
    this->pressureChart->legend()->hide();
}


void MainWindow::attachSeries(){
    this->temperatureChartSeries->attachAxis(this->temperatureTimelineAxis);
    this->temperatureChartSeries->attachAxis(this->temperatureValueAxis);

    this->humidityChartSeries->attachAxis(this->humidityTimelineAxis);
    this->humidityChartSeries->attachAxis(this->humidityValueAxis);

    this->insolationChartSeries->attachAxis(this->insolationTimelineAxis);
    this->insolationChartSeries->attachAxis(this->insolationValueAxis);

    this->pressureChartSeries->attachAxis(this->pressureTimelineAxis);
    this->pressureChartSeries->attachAxis(this->pressureValueAxis);
}


void MainWindow::showChart(){
    this->temperatureChartView = new QChartView(this->temperatureChart);
    this->temperatureChartView->setRenderHint(QPainter::Antialiasing);
    this->temperatureChartView->setParent(ui->temperatureChart_frame);
    ui->temperatureChart_frame->setGeometry(50,50,500,200);

    this->humidityChartView = new QChartView(this->humidityChart);
    this->humidityChartView->setRenderHint(QPainter::Antialiasing);
    this->humidityChartView->setParent(ui->humidityChart_frame);
    ui->humidityChart_frame->setGeometry(50, 50, 500, 200);

    this->insolationChartView = new QChartView(this->insolationChart);
    this->insolationChartView->setRenderHint(QPainter::Antialiasing);
    this->insolationChartView->setParent(ui->insolationChart_frame);
    ui->insolationChart_frame->setGeometry(50, 50, 500, 200);

    this->pressureChartView = new QChartView(this->pressureChart);
    this->pressureChartView->setRenderHint(QPainter::Antialiasing);
    this->pressureChartView->setParent(ui->pressureChart_frame);
    ui->pressureChart_frame->setGeometry(50, 50, 500, 200);
}


void MainWindow::createCharts(){
    this->createSeries();
    this->createAxes();
    this->makeChart();
    this->attachSeries();
    this->showChart();
}
