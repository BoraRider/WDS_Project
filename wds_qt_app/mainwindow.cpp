#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QList>
#include <QDateTime>
#include <iostream>
#include <string.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete device;
}


void MainWindow::showMainBoard(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditBoard->append(currentDateTime + "\t" + message);
}

// ============== F i n g e r s ======================
void MainWindow::showFingerData_1(QString message)
{
    ui->textEditFingerValue_1->append(message);
}
void MainWindow::showFingerData_2(QString message)
{
    ui->textEditFingerValue_2->append(message);
}
void MainWindow::showFingerData_3(QString message)
{
    ui->textEditFingerValue_3->append(message);
}
void MainWindow::showFingerData_4(QString message)
{
    ui->textEditFingerValue_4->append(message);
}
void MainWindow::showFingerData_5(QString message)
{
    ui->textEditFingerValue_5->append(message);
}

// ============== A n g l e s ======================
void MainWindow::showAlphaAngleData(QString message)
{
    ui->textEditAngleAlpha->append(message);
}
void MainWindow::showBetaAngleData(QString message)
{
    ui->textEditAngleBeta->append(message);
}
void MainWindow::showGammaAngleData(QString message)
{
    ui->textEditAngleGamma->append(message);
}

void MainWindow::readFromPort()
{
    while(this->device->canReadLine()) {
      QString line = this->device->readLine();

      QString terminator = "\r";
      int pos = line.lastIndexOf(terminator);

      QString d1 = line.mid(0, 4);
      QString d2 = line.mid(4, 4);
      QString d3 = line.mid(8, 4);
      QString d4 = line.mid(12, 4);
      QString d5 = line.mid(16, 4);
      QString d6 = line.mid(20, 6);
      QString d7 = line.mid(26, 6);
      QString d8 = line.mid(32, 6);


      this->showMainBoard(line.left(pos));
      this->showFingerData_1(d1);
      this->showFingerData_2(d2);
      this->showFingerData_3(d3);
      this->showFingerData_4(d4);
      this->showFingerData_5(d5);
      this->showAlphaAngleData(d6);
      this->showBetaAngleData(d7);
      this->showGammaAngleData(d8);
    }
}



void MainWindow::on_pushButtonConnect_clicked()
{
    this->device->setPortName("COM5");

    if(!device->isOpen())
    {
        if(device->open(QSerialPort::ReadWrite))
        {
            this->device->setBaudRate(QSerialPort::Baud9600);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setParity(QSerialPort::NoParity);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setFlowControl(QSerialPort::NoFlowControl);

            this->showMainBoard("Otwarto port szeregowy.");

            connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
        }
        else
        {
            this->showMainBoard("Otwarcie porty szeregowego się nie powiodło!");
        }
    }
    else{
        this->showMainBoard("Port już jest otwarty!");
        return;
    }
}

void MainWindow::on_pushButtonDisconnect_clicked()
{
    if(this->device->isOpen()) {
      this->device->close();
      this->showMainBoard("Zamknięto połączenie.");
    } else {
      this->showMainBoard("Port nie jest otwarty!");
      return;
    }
}
