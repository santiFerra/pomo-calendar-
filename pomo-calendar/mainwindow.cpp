#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFile"
#include "QIODevice"
#include "QTextStream"
#include "QMessageBox"
#include "QProcess"
#include "QCheckBox"
#include <codecvt>
#include <locale>
#include <string>
#include "QTimer"
#include <iostream>
#include <chrono>
#include <thread>
#include "QThread"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeLeft = ui->textEdit_2->toPlainText().toInt() * 60000;
    timeLeftRestTime = ui->textEdit_3->toPlainText().toInt() * 60000;
    today = ui->calendarWidget->selectedDate().toString();
    //esto no funca porque sucede cuando se crea la main window, debe hacerse luego de ingresar el valor.
    //para eso crear un botton de select/ready/ok para confirmar tiempo seteado. este boton
    //tiene que hacer justo esto de arriba


    QString day = ui->calendarWidget->selectedDate().toString();
    QFile file("/home/sf/calendario");
    if (!file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    //buscar todos las lineas que contengan la fecha seleccionada
    QTextStream in(&file);
    int eventPos = day.size();
    bool moretasks = true;
   //ESTO NO  DEBE ESTAR ACA, porque cambia por el dia
    while(moretasks)
    {
        QString currentEvent;
        currentEvent = in.readLine(0);
        qDebug () << currentEvent;
        if (currentEvent == nullptr)
            moretasks = false;

        if (currentEvent.contains(day))
        {



    //seleccionar de las lineas elegidas unicamente el evento
            currentEvent.chop(eventPos + 1);
    //crear una checkbox por cada uno de los eventos de ese dia.
            QCheckBox* task = new QCheckBox(currentEvent,this);
            ui->todo->addWidget(task);
    //add the hours of that task to the work time of the day
           QChar a =currentEvent.at(currentEvent.size()-1);
           QString dou = a;
           int hoursForTask = dou.toInt();
           workTime+=hoursForTask;
           ui->label->setText("horas por trabajar " + QString::number(workTime));



        }




    }

    file.close();

}
void MainWindow::addEvent()
{
    //Ui::MainWindow* ui = new Ui::MainWindow();
    QString newEvent = ui->textEdit->toPlainText();
    QString eventList;
    QFile file("/home/sf/calendario");
    QTextStream in(&file);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    eventList = in.readAll();
    eventList = eventList + ui->calendarWidget->selectedDate().toString() + newEvent;
    in << eventList;
    file.close();

    //cuando se añade algo a hoy si o si debe sumarse tambien las horas de trabajo extra.






}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_pushButton_clicked()
{


    QString newEvent = ui->textEdit->toPlainText();
    QString eventList;
    QFile file("/home/sf/calendario");
    if (!file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    eventList = newEvent + " " + ui->calendarWidget->selectedDate().toString();
    in.readAll();
    in << "\n"  << eventList ;

    QString day = ui->calendarWidget->selectedDate().toString();
        //buscar todos las lineas que contengan la fecha seleccionada
   //ESTO NO  DEBE ESTAR ACA, porque cambia por el dia

//        newEvent = in.readLine(0);

        if (day == today)
        {


    //seleccionar de las lineas elegidas unicamente el evento
           qDebug() << newEvent;
    //crear una checkbox por cada uno de los eventos de ese dia.
            QCheckBox* task = new QCheckBox(newEvent,this);
            ui->todo->addWidget(task);
    //add the hours of that task to the work time of the day
           QChar z =newEvent.at(newEvent.size()-1);
           QString dou = z;
           int hoursForTask = dou.toInt();
           workTime+=hoursForTask;
           ui->label->setText("horas por trabajar " + QString::number(workTime));



        }





    file.close();

}


void MainWindow::on_calendarWidget_selectionChanged()
{
//    QString day = ui->calendarWidget->selectedDate().toString();
//    QFile file("/home/sf/calendario");
//    if (!file.open(QIODevice::ReadWrite | QFile::Text))
//    {
//        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
//        return;
//    }
//    //buscar todos las lineas que contengan la fecha seleccionada
//    QTextStream in(&file);
//    int eventPos = day.size();
//    bool moretasks = true;
//   //ESTO NO  DEBE ESTAR ACA, porque cambia por el dia
//    while(moretasks)
//    {
//        QString currentEvent;
//        currentEvent = in.readLine(0);
//        if (currentEvent == nullptr)
//            moretasks = false;

//        if (currentEvent.contains(day))
//        {


//    //seleccionar de las lineas elegidas unicamente el evento
//            currentEvent.chop(eventPos + 1);
//    //crear una checkbox por cada uno de los eventos de ese dia.
//            QCheckBox* task = new QCheckBox(currentEvent,this);
//            ui->todo->addWidget(task);
//    //add the hours of that task to the work time of the day
//           QChar a =currentEvent.at(currentEvent.size()-1);
//           char16_t b = a.unicode();
//           char16_t* c = &b;
//           std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
//           std::string str = convert.to_bytes(c);
//           int hoursForTask = std::stoi(str);
//           workTime+=hoursForTask;
//           qDebug() << workTime;
//           ui->label->setText("horas por trabajar " + QString::number(workTime));



//        }




//    }

//    file.close();


}



void MainWindow::on_pushButton_2_clicked()
{
    pomodoro = new QTimer(this);
    pomodoro->setSingleShot(true);
    a = new QTimer(this);
    a->setSingleShot(false);
    connect(a, SIGNAL(timeout()), this, SLOT(update()));
    connect(pomodoro,SIGNAL(timeout()) , this, SLOT(overTime()));
    a->start(1000);
    pomodoro->start(timeLeft);
    int temp = pomodoro->remainingTime()/60000;
    QString timeLeft = QString::number(temp);
    ui->label_2->setText(timeLeft);





}
void MainWindow::update()
{

     int temp = pomodoro->remainingTime()/60000;
     QString timeLeft = QString::number(temp);

     ui->label_2->setText(timeLeft);
//     using namespace std::chrono_literals;
//     std::this_thread::sleep_for(1000ms);
}

void MainWindow::onPushButtomClickedOverTime()
{

   b = new QTimer(this);
   b->start(1000);
   restTime = new QTimer(this);
   restTime->setSingleShot(true);
   restTime->start(timeLeftRestTime);
   connect(b,SIGNAL(timeout()),this,SLOT(updateInOverTime()));
   connect(restTime,SIGNAL(timeout()),this,SLOT(restartPomorodo()));
//       using namespace std::chrono_literals;
//       std::this_thread::sleep_for(7000ms);


}


void MainWindow::overTime()
{
   delete pomodoro;
   delete a;

   ui->label_2->setText("you finished a pomorodo, click start to begin the break");
   disconnect(ui->pushButton_2, SIGNAL(clicked()),0,0);
   connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(onPushButtomClickedOverTime()));

   disconnect(ui->pushButton_3, SIGNAL(clicked()),0,0);
   connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(onStopInRestTime()));
}


void MainWindow::updateInOverTime()
{
   int temp = restTime->remainingTime()/60000;
   QString timeLeft = QString::number(temp);

   ui->label_2->setText(timeLeft);

}

void MainWindow::on_pushButton_3_clicked()
{
  timeLeft = pomodoro->remainingTime();
  delete pomodoro;
  delete a;
}

void MainWindow::onStopInRestTime()
{
    timeLeftRestTime = restTime->remainingTime();
    delete b;
    delete restTime;
}

void MainWindow::restartPomorodo()
{
    //called when restTime finish
    delete b;
    delete restTime;
   ui->label_2->setText("you finished the break, click start to do another pomorodo");
   disconnect(ui->pushButton_2, SIGNAL(clicked()),0,0);
   connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));

   disconnect(ui->pushButton_3, SIGNAL(clicked()),0,0);
   connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(on_pushButton_3_clicked()));
   workTime= workTime - 0.5;
   ui->label->setText("horas por trabajar " + QString::number(workTime));



}

void MainWindow::on_pushButton_4_clicked()
{
   timeLeft = ui->textEdit_2->toPlainText().toInt()*60000;
   timeLeftRestTime = ui->textEdit_3->toPlainText().toInt()*60000;
   ui->label_3->setText("work time: " + QString::number(timeLeft/60000) + "break time: " + QString::number(timeLeftRestTime/60000));
}

