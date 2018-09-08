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


/****************** ATTENTION    *******************/
/*      FORM FOR UART "155 255 123 012\r\n"        */

int timer = 0;
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

    //customPlot->addGraph();




}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::serialReceived()
{
    QByteArray dataCOM;
    int ch[4] = {0,0,0,0};
    dataCOM = serial->readLine();
    ui->textBrowser->insertPlainText(dataCOM);
    qDebug() <<dataCOM;

    //int q = dataCOM.size();
    char *dataChar = dataCOM.data();
    //qDebug() <<q;
    if( ((dataChar[0] == '1') |(dataChar[0] == '2') |(dataChar[0] == '3') |(dataChar[0] == '4') |(dataChar[0] == '5') |
            (dataChar[0] == '6') |(dataChar[0] == '7') |(dataChar[0] == '8') |(dataChar[0] == '9') |(dataChar[0] == '0'))
            & (dataCOM.size() == 17) )
    {
        ch[0] = ((int)dataChar[0] - '0')*100 + ((int)dataChar[1] - '0')*10 +((int)dataChar[2] - '0');
        ch[1] = ((int)dataChar[4] - '0')*100 + ((int)dataChar[5] - '0')*10 +((int)dataChar[6] - '0');
        ch[2] = ((int)dataChar[8] - '0')*100 + ((int)dataChar[9] - '0')*10 +((int)dataChar[10] - '0');
        ch[3] = ((int)dataChar[12] - '0')*100 + ((int)dataChar[13] - '0')*10 +((int)dataChar[14] - '0');

        for (int i = 0; i<5; i++)
        {
         ui->customPlot->graph(0)->addData(timer, ch[0]);
         ui->customPlot_2->graph(0)->addData(timer, ch[1]);
         ui->customPlot_3->graph(0)->addData(timer, ch[2]);
         ui->customPlot_4->graph(0)->addData(timer, ch[3]);

         ui->customPlot->xAxis->setRange(0, timer);
         ui->customPlot_2->xAxis->setRange(0, timer);
         ui->customPlot_3->xAxis->setRange(0, timer);
         ui->customPlot_4->xAxis->setRange(0, timer);

         ui->customPlot->replot();
         ui->customPlot_2->replot();
         ui->customPlot_3->replot();
         ui->customPlot_4->replot();
         timer++;
         //Simulate delay
         // std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    qDebug() <<ch[0];
    qDebug() <<ch[1];
    qDebug() <<ch[2];
    qDebug() <<ch[3];


}
void MainWindow::setupGraph()
{
    ui->customPlot->addGraph();
    ui->customPlot->xAxis->setRange(0, 100);
    ui->customPlot->yAxis->setRange(0, 256);
    //ui->customPlot->xAxis->setLabel("time,s");
    ui->customPlot->yAxis->setLabel("Voltage,V");
    ui->customPlot->show();
    ui->customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->xAxis->setLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->xAxis->setAntialiased(false);




    ui->customPlot_2->addGraph();
    ui->customPlot_2->xAxis->setRange(0, 100);
    ui->customPlot_2->yAxis->setRange(0, 256);
    //ui->customPlot_2->xAxis->setLabel("time,s");
    ui->customPlot_2->yAxis->setLabel("Voltage,V");
    ui->customPlot_2->show();
    ui->customPlot_2->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot_2->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot_2->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot_2->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot_2->graph(0)->rescaleValueAxis();
    ui->customPlot_2->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_2->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_2->xAxis->setLabelFont(QFont(QFont().family(), 8));

    ui->customPlot_3->addGraph();
    ui->customPlot_3->xAxis->setRange(0, 100);
    ui->customPlot_3->yAxis->setRange(0, 256);
    //ui->customPlot_3->xAxis->setLabel("time,s");
    ui->customPlot_3->yAxis->setLabel("Voltage,V");
    ui->customPlot_3->show();
    ui->customPlot_3->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot_3->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot_3->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot_3->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot_3->graph(0)->rescaleValueAxis();
    ui->customPlot_3->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_3->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_3->xAxis->setLabelFont(QFont(QFont().family(), 8));

    ui->customPlot_4->addGraph();
    ui->customPlot_4->xAxis->setRange(0, 100);
    ui->customPlot_4->yAxis->setRange(0, 256);
    ui->customPlot_4->xAxis->setLabel("time,s");
    ui->customPlot_4->yAxis->setLabel("Voltage,V");
    ui->customPlot_4->show();
    ui->customPlot_4->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->customPlot_4->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    ui->customPlot_4->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->customPlot_4->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->customPlot_4->graph(0)->rescaleValueAxis();
    ui->customPlot_4->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_4->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_4->xAxis->setLabelFont(QFont(QFont().family(), 8));

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
    serial->readLine();
    serial->clear();
    timer = 0;

}

/**************************************** Stop TextBrowse ********************************************/
void MainWindow::on_pushButton_6_clicked()
{
    disconnect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
}
