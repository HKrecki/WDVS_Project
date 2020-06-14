#ifndef WEATHERDATAHISTORY_H
#define WEATHERDATAHISTORY_H

#include <QVector>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QtDebug>

/*!
 * \brief Klasa odpowiedzialna za obsługę i wyświetlenie wcześniej zebranych danych pogodowych
 *
 *
 */
class weatherDataHistory
{
public:
    weatherDataHistory();

    /*!
     *  \brief Inicjalizacja i wpisanie do wektorów, zebranych wcześniej danych pogodowych
     */
    void initWeatherHistory();

    /*!
     *  \brief Uśrednienie zebranych danych pogodowych, w stosunku do jednego dnia
     */
    void calculateAverageDayWeatherData();

    char selectWeatherIcon(int t_daysAgoNumber) const;

    double getYesterdayTemperature() const {return yesterdayAverageTemperature;};
    double getTwoDaysAgoTemperature() const {return twoDaysAgoAverageTemperature;};
    double getThreeDaysAgoTemperature() const {return threeDaysAgoAverageTemperature;};
    double getFourDaysAgoTemperature() const {return fourDaysAgoAverageTemperature;};

private:
    struct oneWeatherData{
        float temperature;
        float insolation;
        float rainfall;
    };

    // Wektor zebranych danych z jednego dnia
    QVector<oneWeatherData> yesterdayDataVec;
    QVector<oneWeatherData> twoDaysAgoDataVec;
    QVector<oneWeatherData> threeDaysAgoDataVec;
    QVector<oneWeatherData> fourDaysAgoDataVec;

    // Srednie wartosci pogodowe dla poprzednich dni
    double yesterdayAverageTemperature = 0;
    double yesterdayAverageRainfall = 0;
    double yesterdayAverageInsolation = 0;

    double twoDaysAgoAverageTemperature = 0;
    double twoDaysAgoAverageRainfall = 0;
    double twoDaysAgoAverageInsolation = 0;

    double threeDaysAgoAverageTemperature = 0;
    double threeDaysAgoAverageRainfall = 0;
    double threeDaysAgoAverageInsolation = 0;

    double fourDaysAgoAverageTemperature = 0;
    double fourDaysAgoAverageRainfall = 0;
    double fourDaysAgoAverageInsolation = 0;
};

#endif // WEATHERDATAHISTORY_H
