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
    void on_SearchDevicePushButton_clicked();

    void on_ConnectPushButton_clicked();

    void on_DisconnectPushButton_clicked();

    void readFromPort();

private:
    QSerialPort *device;

    Ui::MainWindow *ui;
    void addToLogs(QString messge);
};
#endif // MAINWINDOW_H
