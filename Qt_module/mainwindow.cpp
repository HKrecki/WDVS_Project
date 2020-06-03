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

    // Inicjalizacja danych o polaczeniu
    initConnectionInformation();
    displayConnectionInformation();


    this->device = new QSerialPort(this);





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



/////////////////////////////////////////////////////////////////////////////
// Obsluga przycisku polacz: polaczenie i przypisanie informacji i polaczeniu
/////////////////////////////////////////////////////////////////////////////
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

            // Ustawienie informacji dotyczacych polaczenia
            ui->ConnectionInformation_TextEdit->clear();

            this->connectionStatus = "Connected";
            this->baudrate = QString::number(this->device->baudRate());
            this->dataBits = QString::number(this->device->dataBits());
            this->parity = "No parity";
            this->stopBits = QString::number(this->device->stopBits());
            this->flowControl = "No flow control";


            connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
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

/////////////////////////////////////////////////////////////////////////////
// Obsluga przycisku rozlacz: polaczenie i przypisanie informacji i polaczeniu
/////////////////////////////////////////////////////////////////////////////
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
}




void MainWindow::readFromPort()
{
    while(this->device->canReadLine()){
        QString line = this->device->readLine();

        QString separator = "\r";
        int pos = line.lastIndexOf(separator);

        this->addToLogs("Get data: " + line.left(pos));
    }
}





