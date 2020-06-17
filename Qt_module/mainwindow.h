#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "weatherdatahistory.h"

#include <QMainWindow>
#include <QDebug>
#include <QList>
#include <QSerialPort>
#include <QDateTime>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

// Obsluga plikow
#include <QFile>
#include <QTextStream>

// Obsluga zegara
#include <QTimer>

// Obsluga wykresow
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>

// Obsluga historii pogody
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*!
     * \brief Metoda, uaktualniająca wykresy o najnowsze zmierzone wartości
     * \param zmienna przechowująca czas, potrzebny do wyznaczenia momentu uaktualnienia
     */
    void updateCharts(unsigned long long t_time);

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pushButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    /*!
     * \brief Slot odpowiedzialny za wyszukiwanie urządzeń, podpiętych do komputera.
     */
    void on_SettingsTabSearchPushButton_clicked();

    /*!
     * \brief Slot odpowiedzialny za połączenie z wybranym urządzeniem
     */
    void on_SettingsTabConnectPushButton_clicked();

    /*!
     * \brief Slot odpowiedzialny za przerwanie połącznia z obecnym urządzeniem
     */
    void on_SettingsTabDisconnectPushButton_clicked();

    /*!
     * \brief Metoda odczytująca dane, przesłane przez stację pogodową
     */
    void readFromPort();


    /*!
     * \brief Metoda odczytuje i wyświetla aktualną datę i godzinę na pulpicie.
     */
    void clockTimerFctn();


private:
    /*!
     * \brief Port przez który podpięte jest urządzenie pomiarowe.
     */
    QSerialPort *device;

    /*!
     * \brief Interfejs graficzny użytkownika.
     */
    Ui::MainWindow *ui;

    /*!
     * \brief Timer wykorzystywany do zliczania czasu rzeczywistego, wyświetlanego na ekranie.
     */
    QTimer *clockTimer;

    /*!
     * \brief Timer wykorzystywany do wyświetlania danych, na wykresach.
     */
    QTime chartTime;

    /*!
     * \brief Metoda ustawiająca detale interfejsu użytkownika
     */
    void setStyle();

    /*!
     * \brief Metoda dodająca do logu, wskazaną wiadomość.
     * \param Wiadomość, która ma być przedstawiona w logu.
     */
    void addToLogs(QString message);

    /*!
     * \brief Metoda inicjalizująca informacje o połączniu, jeszcze przed jego nawiązaniem.
     */
    void initConnectionInformation();

    /*!
     * \brief Metoda wyświetlająca aktualne informacje o nawiązanym połączeniu.
     */
    void displayConnectionInformation();

    /*!
     * \brief Metoda ustawiająca ikonę, wizualizującą stan połączenia.
     * \param Zmienna przekazująca stan połączenia. true - połączenie nawiązane, false - brak połączenia.
     */
    void setConnectionStatusImage(bool t_connectionStatus);

    /*!
     * \brief Metoda odczytująca dane z pliku, w któreym zostały zapisane dane pogodowe z ostatnich dni.
     */
    void readFromFile();

    /*!
     * \brief Metoda odczytujące z jednego pakietu danych i przypisanie wartości do zmiennych.
     */
    void setVariablesFromFileLine();

    /*!
     * \brief Metoda dobirająca i wyświetlająca ikonę, wizualizującą aktualne dane pogodowe.
     * \param Aktualny stan nasłonecznienia.
     * \param Aktualne natężenie opadów.
     */
    void setWeatherIcon(int t_insolation, int t_rainfall);

    /*!
     * \brief Metoda odczytująca i wyświetlająca aktualne szczegółowe dane pogodowe.
     */
    void setDetailWeatherValues();

    /*!
     * \brief Metoda wyświetlająca daty z dni poprzednich.
     */
    void showPastDates();

    /*!
     * \brief Metoda wyświetlające uśrednione tempertatury z dni poprzednich.
     * \param Obiekt przechowujący wszystkie dane pogodowe z danego dnia.
     */
    void showPastTemperature(weatherDataHistory t_pastData);

    /*!
     * \brief Metoda dobierająca i wyświetlająca ikony pogodowe z dni poprzednich.
     * \param Obietk przechowujący wszystkie dane pogodowe z danego dnia.
     */
    void showPastIcons(weatherDataHistory t_pastData);

    // Connection informations
    /*!
     * \brief Zmienna informująca o aktualnym stanie połączenia.
     */
    QString connectionStatus;

    /*!
     * \brief Informacje o aktualnym połączeniu - baudrate.
     */
    QString baudrate;

    /*!
     * \brief Informacje o aktualnym połączeniu - liczba bitów na pakiet.
     */
    QString dataBits;

    /*!
     * \brief Informacje o aktualnym połączeniu - Informacja i systemi sprawdzania parzystości.
     */
    QString parity;

    /*!
     * \brief Informacje o aktualnym stanie połączenia - Liczba bitów stopu.
     */
    QString stopBits;

    /*!
     * \brief Informacje o aktualnym stanie połączenia - Kontrola przepływu.
     */
    QString flowControl;

    /*!
     * \brief Zmienna przechowująca, ostatnio odczytany pakiet danych.
     */
    QString currentRawDataStr;

    /*!
     * \brief Zmienna przechowująca datę i godzinę, w formacie: yyyy:mm:dd_hh:mm.
     */
    QString fullDateStr;

    /*!
     * \brief Zmienna przechowująca datę, w formacie: yyyy.mm.dd.
     */
    QString dateStr;

    /*!
     * \brief Zmienna przechowująca godzinę,w formacie: hh:mm.
     */
    QString hourStr;

    /*!
     * \brief Znak rozpoczynający ramkę danych - $.
     */
    QString startBitStr; // $

    /*!
     * \brief Temperatura odczytana z ramki, typu QString.
     */
    QString temperatureStr;

    /*!
     * \brief Wilgotność powietrza odczytana z ramki danych, typu QString.
     */
    QString humidityStr;

    /*!
     * \brief Ciśnienie atmosferyczne odczytane z ramki danych, typu QString.
     */
    QString pressureStr;

    /*!
     * \brief Intensywność opadów, odczytana z ramki danych, typu QString
     */
    QString rainfallStr;

    /*!
     * \brief Nasłonecznienie, odczytane z ramki danych, typu QString
     */
    QString insolationStr;

    /*!
     * \brief Znak kończący ramkę danych - X
     */
    QString stopBit; // X\r\n

    /*!
     * \brief Aktualnie odczytana temperatura, jednostka: stopnie Celcjusza
     */
    double currentTemperature = 0.0;

    /*!
     * \brief Aktualnie odczytana wilgotność powietrza, jednostaka: [%].
     */
    double currentHumidity = 0.0;

    /*!
     * \brief Aktualnie odczytana wartość ciśnienia atmosferycznego, jednostka: [hPa].
     */
    double currentPressure = 0.0;

    /*!
     * \brief Aktualnie odczytana warość, natężenia opadów, jednostka: [%].
     */
    double currentRainfall = 0.0;

    /*!
     * \brief Aktualnie odczytana warość nasłonecznienia, jednostka: [%].
     */
    double currentInsolation = 0.0;

    // WYKRESY

    // Wyswietlenie

    /*!
     * \brief Widget, wyświetlający wykres temperatury.
     */
    QChartView* temperatureChartView;

    /*!
     * \brief Widget, wyświetlający wykres wilgotności pwoietrza.
     */
    QChartView* humidityChartView;

    /*!
     * \brief Widget, wyświetlający wartości ciśnienia atmosferycznego.
     */
    QChartView* pressureChartView;

    /*!
     * \brief Widget, wyświetlający wartości nasłonecznienia.
     */
    QChartView* insolationChartView;

    // Serie
    /*!
     * \brief Seria danych, dotyczaca wykresu temperatury.
     */
    QLineSeries* temperatureChartSeries;

    /*!
     * \brief Seria danych, dotyczaca wykresu wilgotności powietrza.
     */
    QLineSeries* humidityChartSeries;

    /*!
     * \brief Seria danych, dotycząca wykresu wartości, ciśnienia atmosferycznego.
     */
    QLineSeries* pressureChartSeries;

    /*!
     * \brief Seria danych, dotycząca wykresu wartości, nasłonecznienia.
     */
    QLineSeries* insolationChartSeries;

    // Osie czasu

    /*!
     * \brief Oś czasu, dla wykresu temperatury. Zakres: 00:00 - 24:00 dnia obecnego
     */
    QValueAxis* temperatureTimelineAxis;

    /*!
     * \brief Oś czasu, dla wykresu wilgotności powietrza. Zakres: 00:00 - 24:00 dnia obenego.
     */
    QValueAxis* humidityTimelineAxis;

    /*!
     * \brief Oś czasu, dla wykesu cięniena atmosferycznego. Zakres: 00:00 - 24:00 dnia obecnego.
     */
    QValueAxis* pressureTimelineAxis;

    /*!
     * \brief Oś czasu, dla wykresu nasłonecznienia. Zakres: 00:00 - 24:00 dnia obecnego.
     */
    QValueAxis* insolationTimelineAxis;

    // Osie wartosci

    /*!
     * \brief Oś wartości, dla wykresu temperatury. Zakres: -20,00 - 40,00 [°C].
     */
    QValueAxis* temperatureValueAxis;

    /*!
     * \brief Oś wartości, dla wykresu wilgtności powietrza. Zakres: 0 - 100 [%].
     */
    QValueAxis* humidityValueAxis;

    /*!
     * \brief Oś wartości, dla wykresu ciśnienia atmosferycznego. Zakre: 950 - 1080 [hPa].
     */
    QValueAxis* pressureValueAxis;

    /*!
     * \brief Oś wartości, dla wykresu nasłonecznienia. Zakres: 0 -100 [%].
     */
    QValueAxis* insolationValueAxis;

    // Wykresy
    /*!
     * \brief Wykres temperatury.
     */
    QChart* temperatureChart;

    /*!
     * \brief Wykres wilgotności powietrza.
     */
    QChart* humidityChart;

    /*!
     * \brief Wykres ciśnienia atmosferycznego.
     */
    QChart* pressureChart;

    /*!
     * \brief Wykres nasłonecznienia.
     */
    QChart* insolationChart;

    /*!
     * \brief Metoda dopasowania wykresu do obecnej ramki.
     * \param Zdarzenie zmiany rozmiaru okna.
     */
    void resizeEvent(QResizeEvent* event);


    // WYKRESY - FUNKCJE //

    /*!
     * \brief Utworzenie wszystkich komponentów, wszystkich wykresów.
     */
    void createCharts();

    /*!
     * \brief Utworzenie serii danych dla wszystkich wykresów.
     */
    void createSeries();

    /*!
     * \brief Utworzenie osi czasu i wartości dla wykresów.
     */
    void createAxes();

    /*!
     * \brief Utworzenie wykresów, połączenie osi, serii oraz kompnentów graficznych.
     */
    void makeChart();

    /*!
     * \brief Przyporządkowanie serii do wykresu.
     */
    void attachSeries();

    /*!
     * \brief Wyświetlenie wykresów, w oknie "charts".
     */
    void showChart();

    /*!
     * \brief Inicjalizacja wykresów, wartościami, zebranymi wcześniej, tego samego dnia.
     */
    void initTodayWeatherCharts();


};
#endif // MAINWINDOW_H
