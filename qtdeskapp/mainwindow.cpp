#include <QApplication>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QTextEdit>

#include <QDebug>

QVector<double> x, y;

/******************************************** Main ***************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort (this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())    //read avaible comports
            ui->comboBox->addItem(info.portName());



    //customPlot = new QCustomPlot();
    //ui->widget->addWidget(customPlot,0,0,1,1);  // Устанавливаем customPlot в окно проложения

    //customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    //customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    //customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    //customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    /* Add graph and set the curve line color to green */

}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::serialReceived()
{
    QByteArray data;
    data = serial->readAll();
    //ui->label->setText(data);
    //qDebug() <<data;
    ui->textBrowser->insertPlainText(data);

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
    serial->setBaudRate(QSerialPort::Baud9600);
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
    ui->textBrowser->clear();
    ui->textBrowser->clear();
    ui->textBrowser->clear();
    ui->textBrowser->clear();
    ui->textBrowser->clear();
}

/**************************************** Stop TextBrowse ********************************************/
void MainWindow::on_pushButton_6_clicked()
{
    disconnect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
}
