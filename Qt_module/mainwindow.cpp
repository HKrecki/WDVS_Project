#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <settingstab.h>

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


    // Inicjalizacja danych po polaczeniu
    initConnectionInformation();
    displayConnectionInformation();


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
    }
}



void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.mm.dd hh:mm:ss");
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
    ui->ConnectionInformation_TextEdit->append("Parity:\t\t" + this->parity);
    ui->ConnectionInformation_TextEdit->append("Stop bits:\t\t" + this->stopBits);
    ui->ConnectionInformation_TextEdit->append("Flow control:\t" + this->flowControl);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::setConnectionStatusImage
/// \param t_connectionStatus - Jeśli polaczenie jest aktywne - true, jesli nie - false
///
void MainWindow::setConnectionStatusImage(bool t_connectionStatus)
{
    QString status = "OFF";

    if(t_connectionStatus){
        status = "ON";
    } else{
        status = "OFF";
    }

    QPixmap pix(":/resources/img/wifi"+status+".png");

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

    // ui->test_textEdit->setPlainText(text);

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
/// \brief MainWindow::setWeatherIcon
/// Ustawienie ikony wizualizującej warunki pogodowe
void MainWindow::setWeatherIcon(int t_insolation, int t_rainfall)
{
    int w = ui->WeatherIcon_Label->width();
    int h = ui->WeatherIcon_Label->height();

    // Sloneczna pogoda
    if(t_insolation > 50 && t_rainfall < 10){
        // Slonecznie
        QPixmap pix(":/resources/img/sunny.png");
        ui->WeatherIcon_Label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    }
    else if(t_insolation <= 50 && t_rainfall < 10){ // Pochumrno
        // Pochmurno
        QPixmap pix(":/resources/img/cloudy.png");
        ui->WeatherIcon_Label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    }
    else if(t_rainfall >= 10) { // Deszczowo
        QPixmap pix(":/resources/img/rain.png");
        ui->WeatherIcon_Label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    }

    ui->WeatherIcon_Label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::setDetailWeatherValues
/// Wyswietlenie szczegolowych informacji o aktualnych warunkach pogodowych
void MainWindow::setDetailWeatherValues()
{
    // Aktualna temperatura
    int shortFormatTemperature = temperatureStr.toDouble();
    QString showFormatTemperature;
    showFormatTemperature.setNum(shortFormatTemperature);
    ui->DASHBOARD_temperature_label->setText(showFormatTemperature + "°C");

    // Dane szczegolowe
    ui->DASHBOARD_weatherDetails_label->setText("Humidity:\t" + humidityStr + "%\n"
                                                + "Pressure:\t" + pressureStr + "mbar\n"
                                                + "Insolation:\t" + insolationStr + "%");
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

    // Otwarcie pliku i przygotowanie do zapisu danych


    // TODO: Dodac czyszczenie pliku data.txt
    QFile file("../Qt_Module/data.txt");
    if(!file.open(QFile::ReadWrite | QFile::Text | QIODevice::Append)){
        qDebug() << "Plik nie jest otwarty";
    }

    QTextStream out(&file); // Save to file


    // Odczyt danych z urzadzenia
    while(this->device->canReadLine()){

        currentDateTime = QDateTime::currentDateTime().toString("yyyy.mm.dd hh:mm:ss");
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





