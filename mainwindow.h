#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFileDialog>
#include <QMainWindow>
#include<QVector>
#include<QTextStream>
#include<string>
#include"program.h"
#include<QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void set_result();
    void load(QTextStream &in);

    ~MainWindow();
private:

    Ui::MainWindow *ui;
     program *prolist;
    int cur_linenum;

private slots:




    void on_lineEdit_returnPressed();

    void on_pushButton_3_clicked();
};
#endif // MAINWINDOW_H
