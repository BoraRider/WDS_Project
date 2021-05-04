#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///klasa MainWindow
///
///Główna klasa aplikacji
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// Konstruktor klasy MainWindow.
    /// Ustawia elementy zgodnie z mainwindow.ui.
    /// Alokuje pamięć pod obiekt "device" klasy QSerialPort.
    MainWindow(QWidget *parent = nullptr);

    /// Destruktor klasy MainWindow.
    /// Zwalnia pamięć po obiektach "device" i "ui".
    ~MainWindow();

private slots:

    ///Funkcja readFromPort
    ///
    /// Funkcja czyta pojedynczą linie odebraną z portu szeregowego
    /// i rozdziela odebrane dane na poszczególne elementy (odpowiednie palce i kąty
    /// obrotu)
    void readFromPort();

    /// Funkcja on_pushButtonConnect_clicked
    ///
    /// Funkcja obsługuje sygnał wciśnięcia
    /// przycisku "Połącz".
    /// W funkcji następuje konfiguracja portu szeregowego.
    /// Ustawiany jest na stałe numer portu "COM5".
    /// Szybskość transmisji ustawiana na 9600bps.
    /// Szerokość samki ustawiana na 8 bitów.
    /// Wyłączona obsługa sprawdzenia parzystości.
    /// Ustawiony jeden bit "stop".
    /// Wyłączona kontrola przepływu.
    void on_pushButtonConnect_clicked();

    /// Funkcja on_pushButtonDisconnect_clicked
    ///
    /// Funkcja obsługuje sygnał wciśnięcia
    /// przycisku "Rozłącz".
    /// Zamyka ona połączenie i wyświetla odpowiednie komunikaty oraz
    /// obsługuje błędy wynikające z braku otwartego portu.
    void on_pushButtonDisconnect_clicked();

private:
    /// Zawiera opis interfejsu aplikacji
    Ui::MainWindow *ui;

    /// Klasa odpowiedzialna za komunikacje przezinterfejs UART
    QSerialPort *device;

    /// Funkcja showMainBoard
    ///
    /// Funkcja obsługuje główna plansze w aplikacji.
    /// W aktualnej wersji aplikacji służy ona do wyświetlania stanu połączenia
    /// i odebranych danych wraz z datą i godziną odebrania.
    /// W przyszłości będzie odpowiadać za wyświetlanie modelu 3D dłoni.
    void showMainBoard(QString message);

    /// Funkcja wyświetla dane w zakresie 0-1023 odpowiadające zgjęciu kciuka.
    void showFingerData_1(QString message);

    /// Funkcja wyświetla dane w zakresie 0-1023 odpowiadające zgjęciu palca wskazującego.
    void showFingerData_2(QString message);

    /// Funkcja wyświetla dane w zakresie 0-1023 odpowiadające zgjęciu palca środkowego.
    void showFingerData_3(QString message);

    /// Funkcja wyświetla dane w zakresie 0-1023 odpowiadające zgjęciu palca serdecznego.
    void showFingerData_4(QString message);

    /// Funkcja wyświetla dane w zakresie 0-1023 odpowiadające zgjęciu małego palca.
    void showFingerData_5(QString message);

    /// Funkcja wyświetla dane odpowiadające obrotowi dłoni w osi X
    void showAlphaAngleData(QString message);

    /// Funkcja wyświetla dane odpowiadające obrotowi dłoni w osi Y
    void showBetaAngleData(QString message);

    /// Funkcja wyświetla dane odpowiadające obrotowi dłoni w osi Z
    void showGammaAngleData(QString message);

    /// Zapasowa funkcja odpowiedzialna za wysyłanie danych do mikrokontrolera (nieużywana)
    void sendMessageToDevice(QString message);
};
#endif // MAINWINDOW_H
