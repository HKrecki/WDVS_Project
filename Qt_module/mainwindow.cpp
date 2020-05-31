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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_SearchDevicePushButton_clicked()
{
    QList<QSerialPortInfo> devices;

    qDebug() << "Szukam urządzeń... ";

    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++){
        qDebug() << devices.at(i).portName() << devices.at(i).description();

        ui->DevicesComboBox->addItem(devices.at(i).portName() + " " + devices.at(i).description());
    }








}
