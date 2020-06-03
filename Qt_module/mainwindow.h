#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QList>
#include <QSerialPort>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pushButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_SettingsTabSearchPushButton_clicked();

    void on_SettingsTabConnectPushButton_clicked();

    void on_SettingsTabDisconnectPushButton_clicked();

    void readFromPort();

private:
    QSerialPort *device;
    Ui::MainWindow *ui;
    void addToLogs(QString message);
    void initConnectionInformation();
    void displayConnectionInformation();

    // Connection informations
    QString connectionStatus;
    QString baudrate;
    QString dataBits;
    QString parity;
    QString stopBits;
    QString flowControl;


};
#endif // MAINWINDOW_H
