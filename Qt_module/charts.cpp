/*!
 * \file
 * \brief Definicje funkcji obsługi wykresów
 *
 * Plik zawiera funkcje wyświtelania i obsługi wykesów.
 *
 */




#include "mainwindow.h"



void MainWindow::createSeries(){

    // Customize series
    QPen penT(QRgb(0x000000));
    penT.setWidth(3);

    QPen penH(QRgb(0x000000));
    penH.setWidth(3);

    QPen penI(QRgb(0x000000));
    penI.setWidth(3);

    QPen penP(QRgb(0x000000));
    penP.setWidth(3);

    this->temperatureChartSeries = new QLineSeries();
    this->humidityChartSeries = new QLineSeries();
    this->insolationChartSeries = new QLineSeries();
    this->pressureChartSeries = new QLineSeries();

    temperatureChartSeries->setPen(penT);
    humidityChartSeries->setPen(penH);
    insolationChartSeries->setPen(penI);
    pressureChartSeries->setPen(penP);

}



void MainWindow::createAxes(){
    // Temperature
    this->temperatureTimelineAxis = new QValueAxis;
    this->temperatureTimelineAxis->setLabelFormat("%.2f");
    this->temperatureTimelineAxis->setRange(0,24);
    this->temperatureTimelineAxis->setTickCount(7);

    this->temperatureValueAxis = new QValueAxis;
    this->temperatureValueAxis->setLabelFormat("%.1f");
    this->temperatureValueAxis->setRange(-20,40);
    this->temperatureValueAxis->setTickCount(4);

    QPen axisPen(RGB(255,255,255));
    axisPen.setWidth(2);
    temperatureValueAxis->setLinePen(axisPen);
    temperatureTimelineAxis->setLinePen(axisPen);

    QBrush axisBrush(Qt::white);
    temperatureValueAxis->setLabelsBrush(axisBrush);
    temperatureTimelineAxis->setLabelsBrush(axisBrush);



    // Humidity
    this->humidityTimelineAxis = new QValueAxis;
    this->humidityTimelineAxis->setLabelFormat("%.2f");
    this->humidityTimelineAxis->setRange(0,24);
    this->humidityTimelineAxis->setTickCount(7);

    this->humidityValueAxis = new QValueAxis;
    this->humidityValueAxis->setLabelFormat("%.1f");
    this->humidityValueAxis->setRange(0, 100);
    this->humidityValueAxis->setTickCount(4);

    humidityValueAxis->setLinePen(axisPen);
    humidityTimelineAxis->setLinePen(axisPen);
    humidityValueAxis->setLabelsBrush(axisBrush);
    humidityTimelineAxis->setLabelsBrush(axisBrush);




    // Insolation
    this->insolationTimelineAxis = new QValueAxis;
    this->insolationTimelineAxis->setLabelFormat("%.1f");
    this->insolationTimelineAxis->setRange(0,24);
    this->insolationTimelineAxis->setTickCount(7);

    this->insolationValueAxis = new QValueAxis;
    this->insolationValueAxis->setLabelFormat("%.2f");
    this->insolationValueAxis->setRange(0,100);
    this->insolationValueAxis->setTickCount(4);

    insolationValueAxis->setLinePen(axisPen);
    insolationTimelineAxis->setLinePen(axisPen);
    insolationValueAxis->setLabelsBrush(axisBrush);
    insolationTimelineAxis->setLabelsBrush(axisBrush);


    // Pressure
    this->pressureTimelineAxis = new QValueAxis;
    this->pressureTimelineAxis->setLabelFormat("%.1f");
    this->pressureTimelineAxis->setRange(0, 24);
    this->pressureTimelineAxis->setTickCount(7);

    this->pressureValueAxis = new QValueAxis;
    this->pressureValueAxis->setLabelFormat("%.2f");
    this->pressureValueAxis->setRange(950,1080);
    this->pressureValueAxis->setTickCount(4);

    pressureValueAxis->setLinePen(axisPen);
    pressureTimelineAxis->setLinePen(axisPen);
    pressureValueAxis->setLabelsBrush(axisBrush);
    pressureTimelineAxis->setLabelsBrush(axisBrush);
}

void MainWindow::makeChart(){

    QLinearGradient backgroundGradient;

    this->temperatureChart = new QChart();
    this->temperatureChart->addAxis(temperatureValueAxis,Qt::AlignLeft);
    this->temperatureChart->addAxis(this->temperatureTimelineAxis,Qt::AlignBottom);
    this->temperatureChart->addSeries(this->temperatureChartSeries);
    this->temperatureChart->legend()->hide();

    // Ustawienie tla
    backgroundGradient.setStart(QPointF(0, 0));
    backgroundGradient.setFinalStop(QPointF(0, 1));
    backgroundGradient.setColorAt(0.0,  RGB(61, 62, 64));
    backgroundGradient.setColorAt(1.0,  RGB(61, 62, 64));
    backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    temperatureChart->setBackgroundBrush(backgroundGradient);

    // Ustawienie tla rysowania
    QLinearGradient plotAreaGradient;
    plotAreaGradient.setStart(QPointF(1, 0));
    plotAreaGradient.setFinalStop(QPointF(0, 1));
    plotAreaGradient.setColorAt(1.0, QRgb(0xFF6347));
    plotAreaGradient.setColorAt(0.0, QRgb(0xFFFFFF));
    plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    temperatureChart->setPlotAreaBackgroundBrush(plotAreaGradient);
    temperatureChart->setPlotAreaBackgroundVisible(true);

    // Ustawienie tytulu
    QFont font;
    font.setPixelSize(18);
    temperatureChart->setTitleFont(font);
    temperatureChart->setTitleBrush(QBrush(Qt::white));
    temperatureChart->setTitle("Temperature [°C]");



    // HUMIDITY //
    this->humidityChart = new QChart();
    this->humidityChart->addAxis(humidityValueAxis, Qt::AlignLeft);
    this->humidityChart->addAxis(this->humidityTimelineAxis, Qt::AlignBottom);
    this->humidityChart->addSeries(this->humidityChartSeries);
    this->humidityChart->legend()->hide();

    humidityChart->setBackgroundBrush(backgroundGradient);
    humidityChart->setTitleFont(font);
    humidityChart->setTitleBrush(QBrush(Qt::white));
    humidityChart->setTitle("Humidity [%]");

    // TLO //
    QLinearGradient plotAreaGradientH;
    plotAreaGradientH.setStart(QPointF(1, 0));
    plotAreaGradientH.setFinalStop(QPointF(0, 1));
    plotAreaGradientH.setColorAt(1.0, QRgb(0x3333FF));
    plotAreaGradientH.setColorAt(0.0, QRgb(0x00FFFF));
    plotAreaGradientH.setCoordinateMode(QGradient::ObjectBoundingMode);
    humidityChart->setPlotAreaBackgroundBrush(plotAreaGradientH);
    humidityChart->setPlotAreaBackgroundVisible(true);




    this->insolationChart = new QChart();
    this->insolationChart->addAxis(insolationValueAxis, Qt::AlignLeft);
    this->insolationChart->addAxis(this->insolationTimelineAxis, Qt::AlignBottom);
    this->insolationChart->addSeries(this->insolationChartSeries);
    this->insolationChart->legend()->hide();

    insolationChart->setBackgroundBrush(backgroundGradient);
    insolationChart->setTitleFont(font);
    insolationChart->setTitleBrush(QBrush(Qt::white));
    insolationChart->setTitle("Insolation [%]");

    // TLO //
    QLinearGradient plotAreaGradientI;
    plotAreaGradientI.setStart(QPointF(1, 0));
    plotAreaGradientI.setFinalStop(QPointF(0, 1));
    plotAreaGradientI.setColorAt(1.0, QRgb(0xCC6600));
    plotAreaGradientI.setColorAt(0.0, QRgb(0xFFB266));
    plotAreaGradientI.setCoordinateMode(QGradient::ObjectBoundingMode);
    insolationChart->setPlotAreaBackgroundBrush(plotAreaGradientI);
    insolationChart->setPlotAreaBackgroundVisible(true);



    this->pressureChart = new QChart();
    this->pressureChart->addAxis(pressureValueAxis, Qt::AlignLeft);
    this->pressureChart->addAxis(this->pressureTimelineAxis, Qt::AlignBottom);
    this->pressureChart->addSeries(this->pressureChartSeries);
    this->pressureChart->setTitle("Pressure [hPa]");
    this->pressureChart->legend()->hide();

    pressureChart->setBackgroundBrush(backgroundGradient);
    pressureChart->setTitleFont(font);
    pressureChart->setTitleBrush(QBrush(Qt::white));
    pressureChart->setTitle("Pressure [%]");

    // TLO //
    QLinearGradient plotAreaGradientP;
    plotAreaGradientP.setStart(QPointF(1, 0));
    plotAreaGradientP.setFinalStop(QPointF(0, 1));
    plotAreaGradientP.setColorAt(1.0, QRgb(0xCC00CC));
    plotAreaGradientP.setColorAt(0.0, QRgb(0xFF99FF));
    plotAreaGradientP.setCoordinateMode(QGradient::ObjectBoundingMode);
    pressureChart->setPlotAreaBackgroundBrush(plotAreaGradientP);
    pressureChart->setPlotAreaBackgroundVisible(true);

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
