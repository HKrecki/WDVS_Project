#ifndef WEATHERDATAHISTORY_H
#define WEATHERDATAHISTORY_H

/*!
  * \file
  * \brief Plik zawierający klasę weatheDataHistory, zawierającą zebrane w dniach poprzednich dane.
  */


#include <QVector>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QtDebug>

/*!
 * \brief Klasa odpowiedzialna za obsługę i wyświetlenie wcześniej zebranych danych pogodowych.
 *
 *
 */
class weatherDataHistory
{
public:
    weatherDataHistory();

    /*!
     *  \brief Inicjalizacja i wpisanie do wektorów, zebranych wcześniej danych pogodowych.
     */
    void initWeatherHistory();

    /*!
     *  \brief Uśrednienie zebranych danych pogodowych, w stosunku do jednego dnia.
     */
    void calculateAverageDayWeatherData();

    /*!
     * \brief Funckja ustawiająca ikonę wizualizującą warunki pogodowe, dla ostatnich dni.
     * \param Parametr mówiący dla ilu dni wstecz działa funkcja.
     * \return
     */
    // char selectWeatherIcon(int t_daysAgoNumber) const;

    /*!
     * \brief Funckcja zwracjąca wczorajszą, uśrdenioną temperaturę.
     * \return Uśredniona temperatura z wczoraj
     */
    double getYesterdayTemperature() const {return yesterdayAverageTemperature;};

    /*!
     * \brief Funckja zwracjąca uśrednioną temperaturę, sprzed dwóch dni.
     * \return Uśredniona temperatura sprzed dwóch dni
     */
    double getTwoDaysAgoTemperature() const {return twoDaysAgoAverageTemperature;};

    /*!
     * \brief Funckja zwracjąca uśrednioną temperaturę, sprzed trzech dni.
     * \return Uśredniona temperatura sprzed trzech dni
     */
    double getThreeDaysAgoTemperature() const {return threeDaysAgoAverageTemperature;};

    /*!
     * \brief Funckja zwracjąca uśrednioną temperaturę, sprzed czterech dni.
     * \return Uśredniona temperatura sprzed czterech dni
     */
    double getFourDaysAgoTemperature() const {return fourDaysAgoAverageTemperature;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik natężenia opadów z wczoraj
     * \return Uśredniony wynik natężenia opadów z wczoraj.
     */
    double getYesterdayRainfall() const {return yesterdayAverageRainfall;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik natężenia opadów sprzed dwóch dni
     * \return Uśredniony wynik natężenia opadów z przed dwóch dni.
     */
    double getTwoDaysAgoRainfall() const {return twoDaysAgoAverageRainfall;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik natężenia opadów sprzed trzch dni.
     * \return Uśredniony wynik natężenia opadów sprzed trzech dni.
     */
    double getThreeDaysAgoRainfall() const {return threeDaysAgoAverageRainfall;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik natężenia opadów, sprzed czterech dni.
     * \return Uśredniony wynik natężenia opadów sprzed czterech dni.
     */
    double getFourDaysAgoRainfall() const {return fourDaysAgoAverageRainfall;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik nasłonecznienia, z wczoraj.
     * \return Uśredniony wynik nasłonecznienia z wczoraj.
     */
    double getYesterdayInsolation() const {return yesterdayAverageInsolation;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik nasłonecznienia, sprzed dwóch dni.
     * \return Uśredniony wynik nasłonecznienia sprzed dwóch dni.
     */
    double getTwoDaysAgoInsolation() const {return twoDaysAgoAverageInsolation;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik nasłonecznienia, sprzed trzech dni.
     * \return Uśredniony wynik nasłonecznienia sprzed trzech dni.
     */
    double getThreeDaysAgoInsolation() const {return threeDaysAgoAverageInsolation;};

    /*!
     * \brief Fukcja zwracająca uśredniony wynik nasłonecznienia, sprzed czterech dni.
     * \return Uśredniony wynik nasłonecznienia sprzed czterech dni.
     */
    double getFourDaysAgoInsolation() const {return fourDaysAgoAverageInsolation;};

private:

    /*!
     * \brief Struktura przechowująca dane pogodowe, pobrane z jedneego pakietu.
     */
    struct oneWeatherData{
        float temperature;
        float insolation;
        float rainfall;
    };

    // Wektor zebranych danych z jednego dnia

    /*!
     * \brief Wektor, zawierający wszystkie dane pogodowe, zebrane wczoraj.
     */
    QVector<oneWeatherData> yesterdayDataVec;

    /*!
     * \brief Wektor, zawierający wszystkie dane pogodowe, zebrane dwa dni temu
     */
    QVector<oneWeatherData> twoDaysAgoDataVec;

    /*!
     * \brief Wektor, zawierający wszystkie dane pogodowe, zebrane trzy dni temu.
     */
    QVector<oneWeatherData> threeDaysAgoDataVec;

    /*!
     * \brief Wektor, zawierający wszystkie dane pogodowe, zebrane cztery dni temu.
     */
    QVector<oneWeatherData> fourDaysAgoDataVec;

    // Srednie wartosci pogodowe dla poprzednich dni
    /*!
     * \brief Średnia temperatura z wczoraj.
     */
    double yesterdayAverageTemperature = 0;

    /*!
     * \brief Średnie natężenie opadów z wczoraj.
     */
    double yesterdayAverageRainfall = 0;

    /*!
     * \brief Średnia wartość nasłonecznienia z wczoraj.
     */
    double yesterdayAverageInsolation = 0;

    /*!
     * \brief Średnia temperatura sprzed dwóch dni.
     */
    double twoDaysAgoAverageTemperature = 0;

    /*!
     * \brief Średnie natężenie opadów sprzed dwóch dni.
     */
    double twoDaysAgoAverageRainfall = 0;

    /*!
     * \brief Średnia wartość nasłonecznienia sprzed dwóch dni.
     */
    double twoDaysAgoAverageInsolation = 0;

    /*!
     * \brief Średnia temperatura z sprzed trzech dni.
     */
    double threeDaysAgoAverageTemperature = 0;

    /*!
     * \brief Średnie natężenie opadów sprzed trzech dni.
     */
    double threeDaysAgoAverageRainfall = 0;

    /*!
     * \brief Średnia wartość nasłonecznienia sprzed trzech dni.
     */
    double threeDaysAgoAverageInsolation = 0;

    /*!
     * \brief Średnia temperatura sprzed czterech dni.
     */
    double fourDaysAgoAverageTemperature = 0;

    /*!
     * \brief Średnie natężenie opadów sprzed czterech dni.
     */
    double fourDaysAgoAverageRainfall = 0;

    /*!
     * \brief Średnia wartość nasłonecznienia sprzed czterech dni.
     */
    double fourDaysAgoAverageInsolation = 0;
};

#endif // WEATHERDATAHISTORY_H
