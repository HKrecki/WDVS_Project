/*!
  * \file
  * \brief Plik zawierający funkcję główną programu
  *
  */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/img/WDVS_logo.png"));
    MainWindow w;
    w.show();
    return a.exec();
}
