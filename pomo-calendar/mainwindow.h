#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:

    void addEvent();

    void overTime();

    void onPushButtomClickedOverTime();

    void onStopInRestTime();

    void updateInOverTime();

    void restartPomorodo();

private slots:
    void on_pushButton_clicked();

    void on_calendarWidget_selectionChanged();

    void on_pushButton_2_clicked();

    void update();


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *pomodoro;
    QTimer *a;
    QTimer *b;
    QTimer *restTime;
    int timeLeft = 1500000;
    int timeLeftRestTime = 300000;
    int workTime = 0;
    QString today;
};
#endif // MAINWINDOW_H

