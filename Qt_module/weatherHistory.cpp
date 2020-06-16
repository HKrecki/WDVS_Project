#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::initWeatherHistory
/// Wyświetlenie pogody z ostatnich 4 dni


void MainWindow::initTodayWeatherCharts()
{
    struct oneWeatherData{
        double hour;
        double temperature;
        double humidity;
        double pressure;
        double insolation;
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

    // Data
    int currentLineDay;
    int currentLineMonth;
    int currentLineYear;

    // Godzina
    QString currentLineHourRaw; // Format: hh:mm:ss
    QString currentLineOnlyHour; // Format: hh
    QString currentLineOnlyMinutes; // Foramt: mm
    QString currentLineHourAndMinutes; // Format: hh.mm
    double currentLineHour_double; // Format: hh.mm

    // Dane pogodowe
    double currentLineTemperature;
    double currentLineInsolation;
    double currentLineHumidity;
    double currentLinePressure;

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

        // Przypisanie warunkow pogodowych i godziny do zmiennych:
        allWeatherData = currentLineWeatherData.split("_");
        currentLineHourRaw = allWeatherData.at(0); // hh:mm
        currentLineTemperature = allWeatherData.at(2).toDouble();
        currentLineHumidity = allWeatherData.at(3).toDouble();
        currentLinePressure = allWeatherData.at(4).toDouble();
        currentLineInsolation = allWeatherData.at(6).toDouble();

        // Konwersja godziny na double //
        QStringList hourAndDateList = currentLineHourRaw.split(":");
        currentLineOnlyHour = hourAndDateList.at(0);
        currentLineOnlyMinutes = hourAndDateList.at(1);
        currentLineHourAndMinutes = currentLineOnlyHour + "." + currentLineOnlyMinutes;
        currentLineHour_double = currentLineHourAndMinutes.toDouble(); // Format hh.mm


        // Utworzenie obiektu, przechowujacego pobrane z linii dane pogodowe
        oneWeatherData oneSignalWeatherData;
        oneSignalWeatherData.hour = currentLineHour_double;
        oneSignalWeatherData.temperature = currentLineTemperature;
        oneSignalWeatherData.humidity = currentLineHumidity;
        oneSignalWeatherData.pressure = currentLinePressure;
        oneSignalWeatherData.insolation = currentLineInsolation;


        // Jesli rok, miesiac i dzien sczytanej daty sie zgadza
        if(currentLineYear == yearCur && currentLineMonth == monthCur && currentLineDay == dayCur){
            // Dodanie do wektora dzisiejszego, danych ze sczytanej linii
            todayDataVec.push_back(oneSignalWeatherData);


        }

    }
    file.close();

    // Inicjalizacja wykresow, wartosciami, ktore zostaly zebrane wczesniej, tego samego dnia
    for(int i = 0; i<todayDataVec.size(); i++){
        this->temperatureChartSeries->append(todayDataVec.at(i).hour,todayDataVec.at(i).temperature);
        this->humidityChartSeries->append(todayDataVec.at(i).hour,todayDataVec.at(i).humidity);
        this->pressureChartSeries->append(todayDataVec.at(i).hour,todayDataVec.at(i).pressure);
        this->insolationChartSeries->append(todayDataVec.at(i).hour,todayDataVec.at(i).insolation);
    }

}

