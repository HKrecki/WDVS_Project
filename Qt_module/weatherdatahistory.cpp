#include "weatherdatahistory.h"

weatherDataHistory::weatherDataHistory()
{

}

void weatherDataHistory::initWeatherHistory()
{
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
    float currentLineTemperature;
    float currentLineInsolation;
    float currentLineRainfall;

    QStringList allData;
    QStringList allDateData;
    QStringList allWeatherData;

    QString currentLineDate;
    QString currentLineWeatherData;

    // Czytaj dane do koca pliku, lub do mementu gdy dane beda starsze niz sprzed 4 dni
    while (!in.atEnd() || (currentLineDay < (dayCur-4))) {
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
        currentLineTemperature = allWeatherData.at(2).toFloat();
        currentLineRainfall = allWeatherData.at(5).toFloat();
        currentLineInsolation = allWeatherData.at(6).toFloat();

        // Utworzenie obiektu, przechowujacego pobrane z linii dane pogodowe
        oneWeatherData oneSignalWeatherData;
        oneSignalWeatherData.temperature = currentLineTemperature;
        oneSignalWeatherData.insolation = currentLineInsolation;
        oneSignalWeatherData.rainfall = currentLineRainfall;

        // Zapisanie danych do wektrow, odpowiadajacych danej dacie //
        // Jesli rok i miesiac sczytanej daty sie zgadza
        if(currentLineYear == yearCur && currentLineMonth == monthCur){

            // Obliczenie różnicy dni, miedzy dniem obecnym, a tym odczytanym z linii pliku
            int dayDifference = (dayCur - currentLineDay);

            switch(dayDifference){
            case 1:
                yesterdayDataVec.push_back(oneSignalWeatherData);
                break;
            case 2:
                twoDaysAgoDataVec.push_back(oneSignalWeatherData);
                break;
            case 3:
                threeDaysAgoDataVec.push_back(oneSignalWeatherData);
                break;

            case 4:
                fourDaysAgoDataVec.push_back(oneSignalWeatherData);
                break;
            }
        }
    }
}

/*!
 * \brief Funkcja wyznaczająca średnie warunki pogodowe dla ostatnich dni
 */

void weatherDataHistory::calculateAverageDayWeatherData()
{
    // Obliczenie srednich wartosci z wektorow
    // Odczytanie ilosci zebranych pakietow danych w danym dniu
    int yesterdayDataCount = yesterdayDataVec.size();
    int twoDaysAgoDataCount = twoDaysAgoDataVec.size();
    int threeDaysAgoDataCount = threeDaysAgoDataVec.size();
    int fourDaysAgoDataCount = fourDaysAgoDataVec.size();

    // Usrednienie wartosci dla kolejnych dni //

    // Zsumowanie wartosci
    for(int i = 0; i < yesterdayDataCount; i++){
        yesterdayAverageTemperature += yesterdayDataVec.at(i).temperature;
        yesterdayAverageRainfall += yesterdayDataVec.at(i).rainfall;
        yesterdayAverageInsolation += yesterdayDataVec.at(i).insolation;
    }

    for(int i = 0; i < twoDaysAgoDataCount; i++){
        twoDaysAgoAverageTemperature += twoDaysAgoDataVec.at(i).temperature;
        twoDaysAgoAverageRainfall += twoDaysAgoDataVec.at(i).rainfall;
        twoDaysAgoAverageInsolation += twoDaysAgoDataVec.at(i).insolation;
    }

    for(int i = 0; i < threeDaysAgoDataCount; i++){
        threeDaysAgoAverageTemperature += threeDaysAgoDataVec.at(i).temperature;
        threeDaysAgoAverageRainfall += threeDaysAgoDataVec.at(i).rainfall;
        threeDaysAgoAverageInsolation += threeDaysAgoDataVec.at(i).insolation;
    }

    for(int i = 0; i < fourDaysAgoDataCount; i++){
        fourDaysAgoAverageTemperature += fourDaysAgoDataVec.at(i).temperature;
        fourDaysAgoAverageRainfall += fourDaysAgoDataVec.at(i).rainfall;
        fourDaysAgoAverageInsolation += fourDaysAgoDataVec.at(i).insolation;
    }


    // Usrednienie wartosci
    if(yesterdayDataCount > 0){
        yesterdayAverageTemperature /= yesterdayDataCount;
        yesterdayAverageRainfall /= yesterdayDataCount;
        yesterdayAverageInsolation /= yesterdayDataCount;
    }
    else{
        yesterdayAverageTemperature = -274;
        yesterdayAverageRainfall = -1;
        yesterdayAverageInsolation = -1;
    }


    if(twoDaysAgoDataCount > 0){
        twoDaysAgoAverageTemperature /= twoDaysAgoDataCount;
        twoDaysAgoAverageRainfall /= twoDaysAgoDataCount;
        twoDaysAgoAverageInsolation /= twoDaysAgoDataCount;
    }
    else{
        twoDaysAgoAverageTemperature = -274;
        twoDaysAgoAverageRainfall  = -1;
        twoDaysAgoAverageInsolation = -1;
    }


    if(threeDaysAgoDataCount > 0){
        threeDaysAgoAverageTemperature /= threeDaysAgoDataCount;
        threeDaysAgoAverageRainfall /= threeDaysAgoDataCount;
        threeDaysAgoAverageInsolation /= threeDaysAgoDataCount;
    }
    else{
        threeDaysAgoAverageTemperature = -274;;
        threeDaysAgoAverageRainfall = -1;
        threeDaysAgoAverageInsolation = -1;
    }


    if(fourDaysAgoDataCount > 0){
        fourDaysAgoAverageTemperature /= fourDaysAgoDataCount;
        fourDaysAgoAverageRainfall /= fourDaysAgoDataCount;
        fourDaysAgoAverageInsolation /= fourDaysAgoDataCount;
    }
    else{
        fourDaysAgoAverageTemperature = -274;
        fourDaysAgoAverageRainfall = -1;
        fourDaysAgoAverageInsolation = -1;
    }
}



char weatherDataHistory::selectWeatherIcon(int t_daysAgoNumber) const
{

}








