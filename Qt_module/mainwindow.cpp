#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_SearchDevicePushButton_clicked()
{
    QList<QSerialPortInfo> devices;

    this->addToLogs("Szukam urzadzen... ");

    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++){
        this->addToLogs(devices.at(i).portName() + " " + devices.at(i).description());

        ui->DevicesComboBox->addItem(devices.at(i).portName() + " " + devices.at(i).description());
    }
}


void MainWindow::addToLogs(QString message){
    QString currentDate = QDateTime::currentDateTime().toString("yyyy.mm.dd hh:mm:ss");
    ui->LogsTextEdit->append(currentDate + " " + message);
}






void MainWindow::on_ConnectPushButton_clicked()
{
    if(ui->DevicesComboBox->count() == 0){
        this->addToLogs("Nie wykryto urządzeń");
        return;
    }

    // Podzial aktualnego string w combobox i ustawienie nazwy portu
    QString portName = ui->DevicesComboBox->currentText().split(" ").first();
    this->device->setPortName(portName);

    // Otwarcie i konfiguracja portu
    if(!device->isOpen()){

        if(device->open(QSerialPort::ReadWrite)){

            this->device->setBaudRate(QSerialPort::Baud9600);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setParity(QSerialPort::NoParity);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setFlowControl(QSerialPort::NoFlowControl);

            this->addToLogs("Otwarto port szeregowy ");
        }
        else{
            this->addToLogs("Otwarcie portu nie powiodlo sie ");
        }

    }else{
        this->addToLogs("Ten port jest ju otwarty");
        return;
    }
}








void MainWindow::on_DisconnectPushButton_clicked()
{
    if(this->device->isOpen()){
        this->device->close();
        this->addToLogs("Zamknieto polaczenie");
    }else{
        this->addToLogs("Port nie jest otwarty");
    }
}
