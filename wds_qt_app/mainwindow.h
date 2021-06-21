#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>
#include <QSerialPortInfo>
#include <QList>
#include <QDateTime>
#include <QQuickWidget>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <iostream>
#include <string.h>
#include <sstream>

#define POLYNOMIAL_8 0x2F

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 * Główna klasa aplikacji
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
    * @brief konstruktor MainWindow
    * Ustawia elementy zgodnie z mainwindow.ui.
    * Alokuje pamięć pod obiekt "device" klasy QSerialPort.
    * Przeprowadza wstępną konfigurację wykresów QCustomPlot.
    * Przeprowadza wstępną konfigurację obiektów w warstwie QML oraz linkuje odpowiedni plik.
    */
    MainWindow(QWidget *parent = nullptr);

    /**
    * @brief Destruktor klasy MainWindow.
    *  Zwalnia pamięć po obiektach "device" i "ui".
    */
    ~MainWindow();

private slots:

    /**
    * @brief Funkcja readFromPort
    *  Czyta pojedynczą linie odebraną z portu szeregowego i rozdziela odebrane dane na poszczególne elementy (odpowiednie palce i kąty obrotu).
    */
    void readFromPort();

    /**
    * @brief Funkcja on_pushButtonConnect_clicked
    * Funkcja obsługuje sygnał wciśnięcia przycisku "Połącz".
    * Obsługuje ona nawiązanie połączenia między komputerem a urządzeniem aktualnie wyświetlanym w menu kontekstowym comboBoxDevices
    * W funkcji następuje konfiguracja portu szeregowego:
    * Ustawiany jest numer portu wybrany z menu kontekstowego
    * Szybskość transmisji ustawiana jest na 9600bps.
    * Szerokość samki ustawiana jest na 8 bitów.
    * Wyłączona jest obsługa sprawdzenia parzystości.
    * Ustawiony jest jeden bit "stop".
    * Wyłączona jest kontrola przepływu.
    */
    void on_pushButtonConnect_clicked();


    /** @brief Funkcja on_pushButtonDisconnect_clicked
    * Funkcja obsługuje sygnał wciśnięcia przycisku "Rozłącz".
    * Zamyka ona połączenie i wyświetla odpowiednie komunikaty oraz obsługuje błędy wynikające z braku otwartego portu.
    */
    void on_pushButtonDisconnect_clicked();

    /**
    * @brief Funkcja on_pushButtonSearch_clicked
    * Funkcja obsługuje sygnał wciśnięcia przycisku "Szukaj".
    * Wyszukuje ona znalezione urządzenia podłączone do portów COM.
    * Wyszukane urządzenia pokazywane są w menu kontekstowym comboBoxDevices jako wykryte porty np. "COM5".
    */
    void on_pushButtonSearch_clicked();

    /**
    * @brief Funkcja on_showData_clicked();
    * Funkcja obsługuje sygnał wciśnięcia przycisku "Dane".
    * Zmienia widok aplikacji.
    * Pokazuje plansze z wartościami liczbowymi (zamiast wykresów) oraz dziennik odebranych danych (zamiast widoku 3D).
    */
    void on_showData_clicked();

    /**
    * @brief Funkcja on_showCharts_clicked();
    * Funkcja obsługuje sygnał wciśnięcia przycisku "Wykresy".
    * Zmienia widok aplikacji.
    * Pokazuje plansze z wykresami (zamiast wartości liczbowych) oraz widok 3D dłoni (zamiast dziennika odebranych danych).
    */
    void on_showCharts_clicked();

private:

    /**
     * @brief
     * Klasa odpowiedzialna za układ interfejsu graficznego.
     */
    Ui::MainWindow *ui;
    /**
     * @brief
     * Tablica przechowująca wartości odebrane z uart przekonwertowane do typu int.
     */
    int dOut[9];
    /**
     * @brief
     * Wartość liczbowa typu int tworząca podstawę wykresów.
     */
    int x_loop_counter;
    /**
     * @brief
     * Tablica przechowująca wartości bezpośrednio odebrane z interfejsu szeregowego w formie QString.
     */
    QString d[9];
    /**
     * @brief
     * Wektor tworzący podstawe czasu wykresów.
     */
    QVector<double> general_x_vector;
    /**
     * @brief
     * Tablica wektorów przechowująca historyczne wartości odebranych i przekonwetowanych do int danych.
     */
    QVector<double> v[8];
    /**
     * @brief
     * Tablica char przechowująca ciąg znaków odebranych z interfejsu szeregowego.
     */
    char str[128];
    /**
     * @brief
     * Zmienna pomocnicza do określania długości łańcucha znaków.
     */
    uint8_t str_len=0;
    /**
     * @brief
     * Tablica typu unsigned char przechowująca przekonwertowaną tablice str
     */
    byte arr[128];
    /**
     * @brief
     * Zmienna przechowująca obliczoną sumę kontrolną
     */
    byte sum_crc;
    /**
     * @brief
     * Klasa odpowiedzialna za komunikacje przezinterfejs UART.
     */
    QSerialPort *device;
    /**
    * @brief Funkcja showMainBoard
    * Funkcja służy ona do wyświetlania stanu połączenia i odebranych danych w oknie textEditBoard.
    * @param message Wartość QString zawierająca dane do wyświetlenia w oknie textEditBoard.
    */
    void showMainBoard(QString message);

    /**
    * @brief Funkcja showAllData
    * Funkcja odpowiada za wyswietlanie wszystkich danych odczytanych z czujnikow.
    */
    void showAllData();

    /**
     * @brief CRC8_SingleByte to funkcja pomocnicza głównej funkcji CRC8_DataArray. Oblicza ona pojedyńczy bajt z przekazanego ciągu znaków.
     * @param CRC_prev Zmienna przechowująca pojedyńczy bajt
     * @param Data Zmienna potrzebna przy dzieleniu
     * @return Funkcja zwraca pojedyńczy bajt po przeprowadzeniu dzielenia
     */
    byte CRC8_SingleByte(byte CRC_prev, byte Data);

    /**
     * @brief CRC8_DataArray to główna funkcja licząca sumę kontrolną. Wykorzystuje ona funkcje CRC8_SingleByte.
     * @param pData Ciąg znaków, z których obliczana jest suma kontrolna
     * @param Len Długość łańcucha znaków pData
     * @return Funkcja zwraca obliczoną sumę kontrolną
     */
    byte CRC8_DataArray(byte *pData, byte Len);

    /**
     * @brief string2ByteArray to funkcja zmieniająca ciąg znaków char na unsigned char (byte)
     * @param input Ciąg znaków typu char do konwersji przekazany przez wskaźnik
     * @param output Ciąg znaków typu unsigned char po konwersji przekazany przez wskaźnik
     */
    void string2ByteArray(char* input, byte* output);

protected:

};
#endif // MAINWINDOW_H
