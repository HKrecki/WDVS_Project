#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::initWeatherHistory
/// Wyświetlenie pogody z ostatnich 4 dni


void MainWindow::initTodayWeatherCharts()
{
    struct oneWeatherData{
        double temperature;
        double insolation;
        double rainfall;
    };


    // Wektor zebranych danych z jednego dnia
    QVector<oneWeatherData> todayDataVec;

    // Odczyt aktualnej daty
    QString currentDate;
    currentDate = QDateTime::currentDateTime().toString("yyyy.MM.dd");

    // Podział aktualnej daty na rok, miesiac i dzien -> toInt();
    QStringList fullCurrentDate;
    int yearCur, monthCur, dayCur;

    fullCurrentDate = currentDate.split(".");
    yearCur = fullCurrentDate.at(0).toInt();
    monthCur = fullCurrentDate.at(1).toInt();
    dayCur = fullCurrentDate.at(2).toInt();

    // Otwarcie pliku data.txt
    QFile file("../Qt_Module/data.txt");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Plik nie jest otwarty";
    }

    // Odczytanie linia po lini
    QTextStream in(&file);
    QString currentEntireFileLine;

    // Dane wczytane z linii
    int currentLineDay;
    int currentLineMonth;
    int currentLineYear;
    int currentLineTemperature;
    int currentLineInsolation;
    int currentLineRainfall;

    QStringList allData;
    QStringList allDateData;
    QStringList allWeatherData;

    QString currentLineDate;
    QString currentLineWeatherData;

    while (!in.atEnd()) {
        currentEntireFileLine = in.readLine();

        // Wyciagniecie daty i warunkow z aktualnie czytanej linii
        allData = currentEntireFileLine.split(" ");

        // Podzial odczytanej linii na czesc daty i warnkow pogodowych
        currentLineDate = allData.at(0);
        currentLineWeatherData = allData.at(1);

        // Przypisanie daty do zmiennych: dzien, miesiac, rok
        allDateData = currentLineDate.split(".");
        currentLineYear = allDateData.at(0).toInt();
        currentLineMonth = allDateData.at(1).toInt();
        currentLineDay = allDateData.at(2).toInt();

        // Przypisanie warunkow pogodowych do zmiennych:
        allWeatherData = currentLineWeatherData.split("_");
        currentLineTemperature = allWeatherData.at(2).toInt();
        currentLineRainfall = allWeatherData.at(5).toInt();
        currentLineInsolation = allWeatherData.at(6).toInt();

        qDebug() << "Data: " << currentLineYear << "." << currentLineMonth << "." << currentLineDay;
        qDebug() << "Warunki" << currentLineTemperature << ", " << currentLineMonth
                 << ", " << currentLineDay;

        // Utworzenie obiektu, przechowujacego pobrane z linii dane pogodowe
        oneWeatherData oneSignalWeatherData;
        oneSignalWeatherData.temperature = currentLineTemperature;
        oneSignalWeatherData.insolation = currentLineInsolation;
        oneSignalWeatherData.rainfall = currentLineRainfall;

        // Jesli rok i miesiac sczytanej daty sie zgadza
        if(currentLineYear == yearCur && currentLineMonth == monthCur){
            // Jesli wczoraj
            if(currentLineDay == (dayCur)){
                // Dodanie do wektora wczorajszego, danych ze sczytanej linii
                todayDataVec.push_back(oneSignalWeatherData);
            }
        }
    }

    file.close();
}

