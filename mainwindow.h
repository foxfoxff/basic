#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFileDialog>
#include <QMainWindow>
#include<QVector>
#include<QTextStream>
#include<string>
#include"program.h"

#include"evalstate.h"
#include<QDebug>
#include<QMessageBox>
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
    statement* run_code(statement*);
    void show_help();
    void set_syntree();
    void add_var(QString name,int val);
    void add_str(QString name,QString val);
    bool ifexist(QString name);
    void set_input();
    void handleinput(int);
    void handleLet(int linenum);
    void handlePrint(int linenum);
    void clear();
    bool handleIF(int);    
    void add_syntax(exp*,int);
    void set_syntax();
    void set_var(QString name,int val){
        vals->all[name]=val;
    }
    void set_str(QString name,QString val){
        vals->all_str[name]=val;
    }
    QQueue<statement*> inputline;//要执行输入的行
    ~MainWindow();
  signals:
    void Goto(int);
    void afterinput(statement*);//输入完后发出的信号
public slots:
    void handleGoto(int a);
    //输入完成后继续执行的槽函数
    void continuerun(statement* input){

        run_code(input);
    }

private:
     Ui::MainWindow *ui;
     program *prolist;
     int cur_linenum;
     evalstate *vals;
     int inputnum;
private slots:




    void on_lineEdit_returnPressed();

    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_textBrowser_2_textChanged();
};
#endif // MAINWINDOW_H
