#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort;

    ui->customPlot->addGraph();
    ui->customPlot->yAxis->setRange(-90, 90);
    ui->customPlot->xAxis->setRange(0, 100);

    ui->alphaPlot->addGraph();
    ui->alphaPlot->yAxis->setRange(-90, 90);
    ui->alphaPlot->xAxis->setRange(0, 100);

    ui->betaPlot->addGraph();
    ui->betaPlot->yAxis->setRange(-90, 90);
    ui->betaPlot->xAxis->setRange(0, 100);

    ui->finger1Plot->addGraph();
    ui->finger1Plot->yAxis->setRange(0, 1023);
    ui->finger1Plot->xAxis->setRange(0, 100);

    ui->finger2Plot->addGraph();
    ui->finger2Plot->yAxis->setRange(0, 1023);
    ui->finger2Plot->xAxis->setRange(0, 100);

    ui->finger3Plot->addGraph();
    ui->finger3Plot->yAxis->setRange(0, 1023);
    ui->finger3Plot->xAxis->setRange(0, 100);

    ui->finger4Plot->addGraph();
    ui->finger4Plot->yAxis->setRange(0, 1023);
    ui->finger4Plot->xAxis->setRange(0, 100);

    ui->finger5Plot->addGraph();
    ui->finger5Plot->yAxis->setRange(0, 1023);
    ui->finger5Plot->xAxis->setRange(0, 100);

    x_loop_counter=0;


    ui->quickWidget->setSource(QUrl(QStringLiteral( "qrc:/new/prefix1/main.qml")));

    ui->quickWidget->engine()->rootContext()->setContextProperty("gyro_x", qreal(0));
    ui->quickWidget->engine()->rootContext()->setContextProperty("gyro_y", qreal(0));
    ui->quickWidget->engine()->rootContext()->setContextProperty("gyro_z", qreal(0));
    ui->quickWidget->engine()->rootContext()->setContextProperty("f1", qreal(0));
    ui->quickWidget->engine()->rootContext()->setContextProperty("f2", qreal(0));
    ui->quickWidget->engine()->rootContext()->setContextProperty("f3", qreal(0));
    ui->quickWidget->engine()->rootContext()->setContextProperty("f4", qreal(0));
    ui->quickWidget->engine()->rootContext()->setContextProperty("f5", qreal(0));

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



void MainWindow::showAllData()
{
    ui->finger1Raw->clear();
    ui->finger2Raw->clear();
    ui->finger3Raw->clear();
    ui->finger4Raw->clear();
    ui->finger5Raw->clear();
    ui->alphaRaw->clear();
    ui->betaRaw->clear();
    ui->gammaRaw->clear();

    ui->finger1Raw->append(d[0]);
    ui->finger2Raw->append(d[1]);
    ui->finger3Raw->append(d[2]);
    ui->finger4Raw->append(d[3]);
    ui->finger5Raw->append(d[4]);
    ui->alphaRaw->append(d[5]);
    ui->betaRaw->append(d[6]);
    ui->gammaRaw->append(d[7]);


    if(x_loop_counter>100){
        x_loop_counter = 100;

        general_x_vector.pop_front();

        for(int i=0; i<8; i++){
            v[i].pop_front();
        }

        for(int i=0; i<general_x_vector.size(); i++){
            general_x_vector[i]=general_x_vector[i]-1;
        }

    }
    general_x_vector.append(x_loop_counter);

    for(int i=0; i<8; i++){
        v[i].append(dOut[i]);
    }

    ui->finger1Plot->graph(0)->setData(general_x_vector,v[0]);
    ui->finger1Plot->replot();
    ui->finger1Plot->update();

    ui->finger2Plot->graph(0)->setData(general_x_vector,v[1]);
    ui->finger2Plot->replot();
    ui->finger2Plot->update();

    ui->finger3Plot->graph(0)->setData(general_x_vector,v[2]);
    ui->finger3Plot->replot();
    ui->finger3Plot->update();

    ui->finger4Plot->graph(0)->setData(general_x_vector,v[3]);
    ui->finger4Plot->replot();
    ui->finger4Plot->update();

    ui->finger5Plot->graph(0)->setData(general_x_vector,v[4]);
    ui->finger5Plot->replot();
    ui->finger5Plot->update();

    ui->alphaPlot->graph(0)->setData(general_x_vector,v[5]);
    ui->alphaPlot->replot();
    ui->alphaPlot->update();

    ui->betaPlot->graph(0)->setData(general_x_vector,v[6]);
    ui->betaPlot->replot();
    ui->betaPlot->update();

    ui->customPlot->graph(0)->setData(general_x_vector,v[7]);
    ui->customPlot->replot();
    ui->customPlot->update();

    x_loop_counter++;
}
//==================================================================================
//==================================================================================
//==================================================================================
byte MainWindow::CRC8_SingleByte(byte CRC_prev, byte Data)
{
    CRC_prev ^= Data;
    for(byte Idx=0; Idx<8; ++Idx)
    {
      if((CRC_prev & 0x80) != 0)
      {
        CRC_prev = (byte)((CRC_prev << 1) ^ POLYNOMIAL_8);
      }
      else
      {
        CRC_prev = (byte)(CRC_prev << 1);
      }
    }
    return CRC_prev;
}

byte MainWindow::CRC8_DataArray(byte *pData, byte Len)
{
    byte CRC_final = 0xff;
    for(byte Idx=0; Idx < Len; ++Idx)
    {
      CRC_final = CRC8_SingleByte(CRC_final, pData[Idx]);
    }
    return CRC_final;
}

void MainWindow::string2ByteArray(char *input, byte *output)
{
    uint8_t loop = 0;
    uint8_t i = 0;

    while(input[loop] != '\0')
    {
        output[i++] = input[loop++];
    }
}




void MainWindow::readFromPort()
{
    while(this->device->canReadLine()) {
      QString line = this->device->readLine();

      QString terminator = "\r";
      int pos = line.lastIndexOf(terminator);
      int l=0;

      for(int i=0; i<8; i++){
          d[i] = line.mid(l, 4);
          dOut[i] = d[i].toInt();
          l=l+4;
      }
      d[8] = line.mid(l, 4);


      sprintf(str, "%4d%4d%4d%4d%4d%4d%4d%4d", dOut[0], dOut[1], dOut[2], dOut[3], dOut[4], dOut[5], dOut[6], dOut[7]);

      // zamiana ciągu znaków na ciąg bajtów
      string2ByteArray(str, arr);
      str_len = strlen(str);
      // liczenie sumy kontrolnej
      sum_crc = CRC8_DataArray(arr, str_len);

      //QString s = QString::number(sum_crc);
      //QString suma_kontrolna = "Suma kontrolna: " + s;

      this->showMainBoard(line.left(pos));
      //this->showMainBoard(suma_kontrolna);

      if(sum_crc == d[8].toInt()){
          this->showMainBoard("Odebrano poprawne dane. Suma kontrolna OK.");
          this->showAllData();
          ui->quickWidget->engine()->rootContext()->setContextProperty("gyro_x", qreal(dOut[6]));
          ui->quickWidget->engine()->rootContext()->setContextProperty("gyro_y", qreal(-dOut[7]));
          ui->quickWidget->engine()->rootContext()->setContextProperty("gyro_z", qreal(-dOut[5]));
          ui->quickWidget->engine()->rootContext()->setContextProperty("f1", qreal((dOut[0] * 90)/1024));
          ui->quickWidget->engine()->rootContext()->setContextProperty("f2", qreal((dOut[1] * 90)/1024));
          ui->quickWidget->engine()->rootContext()->setContextProperty("f3", qreal((dOut[2] * 90)/1024));
          ui->quickWidget->engine()->rootContext()->setContextProperty("f4", qreal((dOut[3] * 90)/1024));
          ui->quickWidget->engine()->rootContext()->setContextProperty("f5", qreal((dOut[4] * 90)/1024));
      }
      else{
          this->showMainBoard("Błąd w transmisji. Nie zgadzają się sumy kontrolne.");
      }

    }

}



void MainWindow::on_pushButtonConnect_clicked()
{

    QString portName = ui->comboBoxDevices->currentText();
    this->device->setPortName(portName);

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

void MainWindow::on_pushButtonSearch_clicked()
{
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++) {
      qDebug() << devices.at(i).portName() << devices.at(i).description();
      ui->comboBoxDevices->addItem(devices.at(i).portName());
    }
}

void MainWindow::on_showData_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_showCharts_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
