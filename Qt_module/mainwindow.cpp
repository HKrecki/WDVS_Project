#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include <settingstab.h>

#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Konstruktor okna głównego
/// \param parent
/////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SettingsTabConnectPushButton->setEnabled(false);
    ui->SettingsTabDisconnectPushButton->setEnabled(false);
    setStyle();

    // Ustawienie stylu wykresow



    // Historia warunkow pogodowych //

    // Wyswietlenie danych, zebranych w dniach poprzednich //
    weatherDataHistory pastWeatherData;

    // Ustawienie dat z dni poprzednich
    showPastDates();

    // Sczytanie danych z pliku do pamieci
    pastWeatherData.initWeatherHistory();

    // Usrednienie wynikow z dni poprzednich
    pastWeatherData.calculateAverageDayWeatherData();

    // Wyswietlenie temperatury z dni poprzednich
    showPastTemperature(pastWeatherData);

    // Wyswietlenie ikon pogody z dni poprzednich
    showPastIcons(pastWeatherData);

    // Obsluga wykresow //
    this->chartTime.start(); // Rozpoczeniecie odliczania czasu dla wykresow
    createCharts();

    // Inicjalizacja wykresow, danymi, ktore zostaly odcytane wczesniej, dnia obecnego
    initTodayWeatherCharts();

    // Inicjalizacja danych po polaczeniu
    initConnectionInformation();
    displayConnectionInformation();


    // Przykladowy wykres
    this->device = new QSerialPort(this);

    // Obsulga zegara
    clockTimer = new QTimer(this);
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(clockTimerFctn()));
    clockTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::initCharts
 */
//void MainWindow::initCharts(){
//    this->temperatureChartSeries->append(0, 0);
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Aktualizacja i dodanie wartosci do wykresow, zmiana zakresu osi czasu
/// \param t_temperature
/// \param t_humidity
/// \param t_pressure
/// \param t_insolation
/// \param t_time
/// Aktualizacja i dodanie wartosci do wykresow, zmiana zakresu osi czasu

void MainWindow::updateCharts(unsigned long long t_time)
{
    QTime time = QTime::currentTime();
    QString timeStr = time.toString("hh.mm");

    float hour = timeStr.toFloat();


    // Wstawienie nowych punktow do wykresu
    this->temperatureChartSeries->append(hour, this->currentTemperature);
    this->humidityChartSeries->append(hour, this->currentHumidity);
    this->insolationChartSeries->append(hour, this->currentInsolation);
    this->pressureChartSeries->append(hour, this->currentPressure);
}



void MainWindow::on_tabWidget_currentChanged(int index)
{

}


void MainWindow::on_SettingsTabSearchPushButton_clicked()
{
    ui->SettingsTabDevices_ComboBox->clear();

    QList<QSerialPortInfo> devices;
    addToLogs("Searching devices... ");

    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++){
        addToLogs("Found: " + devices.at(i).portName() + " " + devices.at(i).description());
        ui->SettingsTabDevices_ComboBox->addItem(devices.at(i).portName() + " " + devices.at(i).description());
        ui->SettingsTabConnectPushButton->setEnabled(true);
    }
}



void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->Logs_TextEdit->append(currentDateTime + "\t\t" + message);
}


//////////////////////////////////
// Obsluga informacji o polaczeniu
//////////////////////////////////
void MainWindow::initConnectionInformation()
{
    this->connectionStatus = "Not connected";
    this->baudrate = "---";
    this->dataBits = "---";
    this->parity = "---";
    this->stopBits = "---";
    this->flowControl = "---";

    this->setConnectionStatusImage(false);
    ui->statusbar->showMessage("Disconnected");
}

void MainWindow::displayConnectionInformation()
{
    ui->ConnectionInformation_TextEdit->clear();

    ui->ConnectionInformation_TextEdit->append("Connection Status:\t" + this->connectionStatus);
    ui->ConnectionInformation_TextEdit->append("Baudrate:\t\t" + this->baudrate);
    ui->ConnectionInformation_TextEdit->append("Data bits:\t\t" + this->dataBits);
    ui->ConnectionInformation_TextEdit->append("Parity:\t\t\t" + this->parity);
    ui->ConnectionInformation_TextEdit->append("Stop bits:\t\t" + this->stopBits);
    ui->ConnectionInformation_TextEdit->append("Flow control:\t\t" + this->flowControl);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::setConnectionStatusImage
/// \param t_connectionStatus - Jeśli polaczenie jest aktywne - true, jesli nie - false
///
void MainWindow::setConnectionStatusImage(bool t_connectionStatus)
{
    QString status = "OFF";

    if(t_connectionStatus){
        status = "On";
    } else{
        status = "Off";
    }

    QPixmap pix(":/resources/img/wifi"+status+"_white.png");

    int w = ui->ConnectionStatusImage_Label->width();
    int h = ui->ConnectionStatusImage_Label->height();

    ui->ConnectionStatusImage_Label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    ui->ConnectionStatusImage_Label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

}

void MainWindow::readFromFile()
{
    QFile file("../Qt_Module/data.txt");
    if(!file.open(QFile::ReadWrite | QFile::Text | QIODevice::Append)){
        qDebug() << "Plik nie jest otwarty";
    }

    QTextStream in(&file); // Save to file
    QString text = in.readAll();

    file.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::setVariablesFromFileLine
/// Przydział wartości zmiennych, wydzielonych z ramki danych
void MainWindow::setVariablesFromFileLine()
{
    // Podzial ramki danych na czytelne elementy
    QStringList allDataList = currentRawDataStr.split("_", QString::SkipEmptyParts);
    QStringList entireDateList;

    // Sprawdzenie poprawnosci przslanych danych
    if(allDataList.at(1) == "$" && allDataList.at(7) == "X\r\n" && allDataList.count() == 8){

        // Przypisanie wartosci do zmiennych typu QString
        this->fullDateStr = allDataList.at(0);
        this->temperatureStr = allDataList.at(2);
        this->humidityStr = allDataList.at(3);
        this->pressureStr = allDataList.at(4);
        this->rainfallStr = allDataList.at(5);
        this->insolationStr = allDataList.at(6);
    }else{
        qDebug() << "Przeslana ramka jest niepoprawna";
    }

    // Podzielenie daty na dzien i godzine
    entireDateList = fullDateStr.split(" ", QString::SkipEmptyParts);
    this->dateStr = entireDateList.at(0);
    this->hourStr = entireDateList.at(1);

    bool ok; // To check conversion failure

    // Przypisaenie wartosci liczbowych do zmiennych liczbowych
    this->currentTemperature = temperatureStr.split(" ")[0].toDouble();
    this->currentHumidity = humidityStr.split(" ")[0].toDouble();
    this->currentPressure = pressureStr.split(" ")[0].toDouble();
    this->currentRainfall = rainfallStr.split(" ")[0].toDouble();
    this->currentInsolation = insolationStr.split(" ")[0].toDouble(&ok);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Ustawienie ikony wizualizującej aktualne warunki pogodowe
void MainWindow::setWeatherIcon(int t_insolation, int t_rainfall)
{
    int w = ui->WeatherIcon_Label->width();
    int h = ui->WeatherIcon_Label->height();

    // Sloneczna pogoda
    if(t_insolation > 50 && t_rainfall < 10){
        // Slonecznie
        QPixmap pix(":/resources/img/sun.png");
        ui->WeatherIcon_Label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    }
    else if(t_insolation <= 50 && t_rainfall < 10){ // Pochumrno
        // Pochmurno
        QPixmap pix(":/resources/img/cloud.png");
        ui->WeatherIcon_Label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    }
    else if(t_rainfall >= 10) { // Deszczowo
        QPixmap pix(":/resources/img/rainy.png");
        ui->WeatherIcon_Label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    }

    ui->WeatherIcon_Label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Wyswietlenie szczegolowych informacji o aktualnych warunkach pogodowych
void MainWindow::setDetailWeatherValues()
{
    // Aktualna temperatura
    int shortFormatTemperature = temperatureStr.toDouble();
    QString showFormatTemperature;
    showFormatTemperature.setNum(shortFormatTemperature);
    ui->DASHBOARD_temperature_label->setText(showFormatTemperature + "°C");

    // Dane szczegolowe
    ui->DASHBOARD_weatherDetails_label->setText("Humidity:\t" + humidityStr + "%\n"
                                                + "Pressure:\t" + pressureStr + " hPa\n"
                                                + "Insolation:\t" + insolationStr + "%");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Ustawienie i wyświetlenie skróconych dat, dni poprzednich
///
void MainWindow::showPastDates()
{
    QLocale curLocale(QLocale("en_US"));
    QLocale::setDefault(curLocale);

    QDate date = QDate::currentDate();
    QString englishDate = QLocale().toString(date);

    QStringList devideDate = englishDate.split(", ");
    QStringList MonthAndDay = devideDate.at(1).split(" ");

    int todayDay = MonthAndDay.at(1).toInt();
    QString oneDayAgo = QString::number(todayDay - 1);
    QString twoDaysAgo = QString::number(todayDay - 2);
    QString threeDaysAgo = QString::number(todayDay - 3);
    QString fourDaysAgo = QString::number(todayDay - 4);

    // Set short dates
    QString oneDayAgoShortDate = MonthAndDay.at(0) + " " + oneDayAgo;
    QString twoDaysAgoShortDate = MonthAndDay.at(0) + " " + twoDaysAgo;
    QString threeDaysAgoShortDate = MonthAndDay.at(0) + " " + threeDaysAgo;
    QString fourDaysAgoShortDate = MonthAndDay.at(0) + " " + fourDaysAgo;

    ui->oneDayAgoDate_label->setText(oneDayAgoShortDate);
    ui->twoDaysAgoDate_label->setText(twoDaysAgoShortDate);
    ui->threeDaysAgoDate_label->setText(threeDaysAgoShortDate);
    ui->fourDaysAgoDate_label->setText(fourDaysAgoShortDate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Wyświetlenie uśrednionej temperatury z dni poprzednich.
/// \param Obiekt przechowujący informacje pogodowe z dni poprzednich.
///
void MainWindow::showPastTemperature(weatherDataHistory t_pastData)
{
    int yesterdayTemperatureInt = t_pastData.getYesterdayTemperature();
    int twoDaysAgoTemperatureInt = t_pastData.getTwoDaysAgoTemperature();
    int threeDaysAgoTemperatureInt = t_pastData.getThreeDaysAgoTemperature();
    int fourDaysAgoTemperatureInt = t_pastData.getFourDaysAgoTemperature();

    // TODO: Wyswietlenie no data w przypadku braku informacji, gdy temperatura = -100
    if(yesterdayTemperatureInt <= -274)
        ui->yesterdayTemperature_label->setText("No data");
    else
        ui->yesterdayTemperature_label->setText(QString::number(yesterdayTemperatureInt) + "°C");


    if(twoDaysAgoTemperatureInt <= -274)
        ui->twoDaysAgoTemperature_label->setText("No data");
    else
        ui->twoDaysAgoTemperature_label->setText(QString::number(twoDaysAgoTemperatureInt) + "°C");


    if(threeDaysAgoTemperatureInt <= -274)
        ui->threeDaysAgoTemeperature_label->setText("No data");
    else
        ui->threeDaysAgoTemeperature_label->setText(QString::number(threeDaysAgoTemperatureInt) + "°C");


    if(fourDaysAgoTemperatureInt <= -274)
        ui->fourDaysAgoTemperature_label->setText("No data");
    else
        ui->fourDaysAgoTemperature_label->setText(QString::number(fourDaysAgoTemperatureInt) + "°C");
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Ustawia ikonę pogodową dla poprzednich dni, w zależności, od zebranych do 4 dni temu, danych.
/// \param obiekt klasy weatherDataHistory, przechowujący dane pogodowe z danego dnia.
///
///
void MainWindow::showPastIcons(weatherDataHistory t_pastData)
{
    int _currentRainfall = 0;
    int _currentInsolation = 0;

    // Zmienne do ustawienia rozmiaru ikony
    int _w = ui->Day1ago_label->width();
    int _h = ui->Day1ago_label->height();

    QPixmap pix;
    pix.load(":/resources/img/XnoData.png");

    for(int i =0; i<4; i++){
        switch(i){
            case 0:
                _currentRainfall = t_pastData.getYesterdayRainfall();
                _currentInsolation = t_pastData.getYesterdayInsolation();
                break;
            case 1:
                _currentRainfall = t_pastData.getTwoDaysAgoRainfall();
                _currentInsolation = t_pastData.getTwoDaysAgoInsolation();
                break;
            case 2:
                _currentRainfall = t_pastData.getThreeDaysAgoRainfall();
                _currentInsolation = t_pastData.getThreeDaysAgoInsolation();
                break;
            case 3:
                _currentRainfall = t_pastData.getFourDaysAgoRainfall();
                _currentInsolation = t_pastData.getFourDaysAgoInsolation();
                break;
        }



        if(_currentInsolation > 50 && _currentRainfall >= 0 && _currentRainfall < 10){
            pix.load(":/resources/img/sun.png");
        }
        else if((_currentInsolation <= 50 && _currentInsolation >= 0)
                && (_currentRainfall < 10 && _currentRainfall >= 0)){
            pix.load(":/resources/img/cloud.png");
        }
        else if(_currentRainfall >= 10){
            pix.load(":/resources/img/rainy.png");
        }
        else if(_currentRainfall <= -1 && _currentInsolation <= -1){
            pix.load(":/resources/img/XnoData.png");
        }


        switch(i){
            case 0:
                ui->Day1ago_label->setPixmap(pix.scaled(_w, _h, Qt::KeepAspectRatio));
                break;
            case 1:
                ui->Day2ago_label->setPixmap(pix.scaled(_w, _h, Qt::KeepAspectRatio));
                break;
            case 2:
                ui->Day3ago_label->setPixmap(pix.scaled(_w, _h, Qt::KeepAspectRatio));
                break;
            case 3:
                ui->Day4ago_label->setPixmap(pix.scaled(_w, _h, Qt::KeepAspectRatio));
                break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_SettingsTabConnectPushButton_clicked
/// Obsługa przycisku połącz - Nawiązanie połączenia z urządzeniem
void MainWindow::on_SettingsTabConnectPushButton_clicked()
{
    if(ui->SettingsTabDevices_ComboBox->count() == 0){
        this->addToLogs("No devices detected");
    }

    QString portName = ui->SettingsTabDevices_ComboBox->currentText().split(" ").first();
    this->device->setPortName(portName);

    if(!device->isOpen()){
        if(device->open(QSerialPort::ReadWrite)){
            this->device->setBaudRate(9600);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setParity(QSerialPort::NoParity);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setFlowControl(QSerialPort::NoFlowControl);



            this->addToLogs("The serial port is now open");
            ui->SettingsTabConnectPushButton->setEnabled(false);
            ui->SettingsTabDisconnectPushButton->setEnabled(true);

            // Ustawienie ikony symbolizujacej status polaczenia - ON
            setConnectionStatusImage(true);

            // Ustawienie informacji dotyczacych polaczenia
            ui->ConnectionInformation_TextEdit->clear();

            this->connectionStatus = "Connected";
            this->baudrate = QString::number(this->device->baudRate());
            this->dataBits = QString::number(this->device->dataBits());
            this->parity = "No parity";
            this->stopBits = QString::number(this->device->stopBits());
            this->flowControl = "No flow control";


            connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
            ui->statusbar->showMessage("Connected");
        }else{
            this->addToLogs("The opening of the serial port has failed");
        }
    }
    else{
        this->addToLogs("The port is already open");
    }


    // Wyswietlenie informacji o polaczeniu w oknie
    this->displayConnectionInformation();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::on_SettingsTabDisconnectPushButton_clicked
/// Obsługa przycisku rozłącz - Rozłączenie z urządzeniem
void MainWindow::on_SettingsTabDisconnectPushButton_clicked()
{
    if(this->device->isOpen()){
        this->device->close();
        this->addToLogs("The port has been closed ");
        ui->SettingsTabDisconnectPushButton->setEnabled(false);
        ui->SettingsTabConnectPushButton->setEnabled(true);
    } else{
        this->addToLogs("The port is already closed ");
    }

    ui->ConnectionInformation_TextEdit->clear();
    this->initConnectionInformation();
    this->displayConnectionInformation();

    // Ustawienie ikony symbolizującej status polaczenia - OFF
    setConnectionStatusImage(false);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::readFromPort
/// Odczytanie danych z urządzenia
void MainWindow::readFromPort()
{
    // Zmienna przechowujaca aktualna date
    QString currentDateTime;

    // Czas potrzebny do wykresow
    unsigned long long elapsedTime;


    // TODO: Dodac czyszczenie pliku data.txt
    QFile file("../Qt_Module/data.txt");
    if(!file.open(QFile::ReadWrite | QFile::Text | QIODevice::Append)){
        qDebug() << "Plik nie jest otwarty";
    }

    QTextStream out(&file); // Save to file


    // Odczyt danych z urzadzenia
    while(this->device->canReadLine()){

        elapsedTime = (unsigned long long) this->chartTime.elapsed();

        currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
        QString line = this->device->readLine();

        QString separator = "\r";
        int pos = line.lastIndexOf(separator);

        this->addToLogs("Get data: " + line.left(pos));

        // Zapis ramki danych do pliku
        out << currentDateTime + "_" + line;

        // Odczyt calej lini i zapis do zmiennej, przeznaczonej do pozniejszego podzielenia
        currentRawDataStr = currentDateTime+"_"+line;

        // Podzial odczytanej lini na potrzebne elementy(wartosci)
        this->setVariablesFromFileLine();

        // Ustawienie odpowiedniej wizualizacji pogody
        setWeatherIcon(currentInsolation, currentRainfall);

        // Ustawienie aktualnych wartosci szczegolowych warunkow pogodowych
        setDetailWeatherValues();

        // Aktualizacja danych na wykresie
        this->updateCharts(elapsedTime);
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::clockTimerFctn
/// Obsługa daty wyświetlanej na pulpicie
void MainWindow::clockTimerFctn()
{
    QLocale curLocale(QLocale("en_US"));
    QLocale::setDefault(curLocale);

    QTime time = QTime::currentTime();
    QString timeStr = time.toString("hh : mm");

    QDate date = QDate::currentDate();
    QString englishDate = QLocale().toString(date);

    ui->DASHBOARD_clock_label->setText(timeStr);
    ui->DASHBOARD_date_label->setText(englishDate);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Ustawia wygląd interfejsu
///

void MainWindow::setStyle()
{
    ui->statusbar->setStyleSheet("color: rgb(255, 255, 255);");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::resizeEvent
/// \param event
/// Zmiana wielkosci wykresu w zaleznosci od zmiany rozmiaru okna(rodzica)
void MainWindow::resizeEvent(QResizeEvent* event){

    QMainWindow::resizeEvent(event);

    this->temperatureChartView->resize(this->temperatureChartView->parentWidget()->size());
    this->humidityChartView->resize(this->humidityChartView->parentWidget()->size());
    this->insolationChartView->resize(this->insolationChartView->parentWidget()->size());
    this->pressureChartView->resize(this->pressureChartView->parentWidget()->size());
}

void MainWindow::on_pushButton_clicked(){

}

void MainWindow::on_tabWidget_tabBarClicked(int a){

}
