#include <QApplication>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QTextEdit>
#include <chrono>
#include <thread>
#include <QDataStream>
#include <QDebug>

#define LENGHT 6700
/****************** ATTENTION    *******************/
/*     ASCII FORM FOR UART "155 255 123 012\r\n"        */
/*     HEX FORM FOR UART "'a'0xff0xff0xff0xff\n"        */


QVector<double> timer(LENGHT);
QVector<double> dataCh1(LENGHT);
QVector<double> dataCh2(LENGHT);
QVector<double> dataCh3(LENGHT);
QVector<double> dataCh4(LENGHT);
int timCount = 0;
/******************************************** Main ***************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort (this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())    //read avaible comports
            ui->comboBox->addItem(info.portName());

    customPlot = new QCustomPlot();
    for(int i = 0; i < LENGHT; i++){
        timer[i] = i+1;
        dataCh1[i] = 0;
        dataCh2[i] = 0;
        dataCh3[i] = 0;
        dataCh4[i] = 0;
    }
    setupGraph();

    //customPlot->addGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::serialReceived(){

    QByteArray dataCOM;
    dataCOM = serial->readAll();
    //ui->textBrowser->insertPlainText(dataCOM);
    qDebug() << "sizeof(dataCOM) = " << dataCOM.size();

    for(int i = 0; i < dataCOM.size(); i++){
        if(timCount == LENGHT){
            timCount = 0;
        }
        if(dataCOM.size() - i < 5){
            i=dataCOM.size();
            dataCh1[timCount] = dataCh1[timCount-1];
            dataCh2[timCount] = dataCh2[timCount-1];
            dataCh3[timCount] = dataCh3[timCount-1];
            dataCh4[timCount] = dataCh4[timCount-1];
            timCount++;
        }
        if(timCount > LENGHT-10){
            timCount = 0;
        }
        if(dataCOM[i]=='a'){
            i++;
            dataCh1[timCount] = (uint8_t)dataCOM[i];
            i++;
            dataCh2[timCount] = (uint8_t)dataCOM[i];
            i++;
            dataCh3[timCount] = (uint8_t)dataCOM[i];
            i++;
            dataCh4[timCount] = (uint8_t)dataCOM[i];
            timCount++;
        }
    }
    if(timCount%10==0){
        ui->customPlot->graph(0)->setData(timer, dataCh1);
        ui->customPlot_2->graph(0)->setData(timer, dataCh2);
        ui->customPlot_3->graph(0)->setData(timer, dataCh3);
        ui->customPlot_4->graph(0)->setData(timer, dataCh4);
        ui->customPlot->replot();
        ui->customPlot_2->replot();
        ui->customPlot_3->replot();
        ui->customPlot_4->replot();
    }
}
/**************************************** Button Send ************************************************/
void MainWindow::on_pushButton_clicked()
{
    //string for lineedit text
    QString str2 = ui->lineEdit->text();
    serial->write(str2.toUtf8());
}

/*************************************** Button Connect **********************************************/
void MainWindow::on_pushButton_2_clicked()
{
    //select name for our serial port from combobox
    if (serial->portName() != ui->comboBox->currentText())
    {
          serial->close();
          serial->setPortName(ui->comboBox->currentText());
    }
    //setup COM port
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setBaudRate(921600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QSerialPort::ReadWrite);


}

/**************************************** Button Get *************************************************/
void MainWindow::on_pushButton_4_clicked()
{
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
    setupGraph();
}
/************************************** Button Close Port ********************************************/
void MainWindow::on_pushButton_3_clicked()
{
    //close
    serial->close();
}

/*************************************** Clean TextBrowse ********************************************/
void MainWindow::on_pushButton_5_clicked()
{
    disconnect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));

    ui->textBrowser->clear();
    ui->textBrowser->clear();
    ui->textBrowser->clear();
    ui->textBrowser->clear();
    ui->textBrowser->clear();
    ui->customPlot->clearGraphs();
    ui->customPlot_2->clearGraphs();
    ui->customPlot_3->clearGraphs();
    ui->customPlot_4->clearGraphs();
    ui->customPlot->xAxis->setRange(0, LENGHT);
    ui->customPlot_2->xAxis->setRange(0, LENGHT);
    ui->customPlot_3->xAxis->setRange(0, LENGHT);
    ui->customPlot_4->xAxis->setRange(0, LENGHT);

    serial->readLine();
    serial->clear();
    timCount = 0;

}

/**************************************** Stop TextBrowse ********************************************/
void MainWindow::on_pushButton_6_clicked()
{
    disconnect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
}

void MainWindow::setupGraph()
{
    ui->customPlot->addGraph();
    ui->customPlot->xAxis->setRange(0, LENGHT);
    ui->customPlot->yAxis->setRange(0, 256);
    //ui->customPlot->xAxis->setLabel("time,s");
    ui->customPlot->yAxis->setLabel("Voltage,V");
    ui->customPlot->show();
    ui->customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->xAxis->setLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->xAxis->setAntialiased(false);

    ui->customPlot_2->addGraph();
    ui->customPlot_2->xAxis->setRange(0, LENGHT);
    ui->customPlot_2->yAxis->setRange(0, 256);
    //ui->customPlot_2->xAxis->setLabel("time,s");
    ui->customPlot_2->yAxis->setLabel("Voltage,V");
    ui->customPlot_2->show();
    ui->customPlot_2->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot_2->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot_2->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot_2->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot_2->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_2->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_2->xAxis->setLabelFont(QFont(QFont().family(), 8));

    ui->customPlot_3->addGraph();
    ui->customPlot_3->xAxis->setRange(0, LENGHT);
    ui->customPlot_3->yAxis->setRange(0, 256);
    //ui->customPlot_3->xAxis->setLabel("time,s");
    ui->customPlot_3->yAxis->setLabel("Voltage,V");
    ui->customPlot_3->show();
    ui->customPlot_3->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot_3->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot_3->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot_3->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot_3->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_3->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_3->xAxis->setLabelFont(QFont(QFont().family(), 8));

    ui->customPlot_4->addGraph();
    ui->customPlot_4->xAxis->setRange(0, LENGHT);
    ui->customPlot_4->yAxis->setRange(0, 256);
    ui->customPlot_4->xAxis->setLabel("time,s");
    ui->customPlot_4->yAxis->setLabel("Voltage,V");
    ui->customPlot_4->show();
    ui->customPlot_4->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot_4->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot_4->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot_4->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot_4->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_4->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_4->xAxis->setLabelFont(QFont(QFont().family(), 8));

    ui->customPlot->graph(0)->setData(timer, dataCh1);
    ui->customPlot_2->graph(0)->setData(timer, dataCh2);
    ui->customPlot_3->graph(0)->setData(timer, dataCh3);
    ui->customPlot_4->graph(0)->setData(timer, dataCh4);

}
