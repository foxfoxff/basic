#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTextBlock>

#include<program.h>
#include <string>
#include<sstream>
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    bool ok;
    QString a="-1";
    qDebug()<<a.toInt(&ok);
    qDebug()<<ok;
    ui->setupUi(this);
    this->setWindowTitle("Basic");
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setSizePolicy(sizePolicy);
    prolist= new program;
    vals = new evalstate;
    cur_linenum=0;
    ui->lineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lineEdit->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_result(){
      ui->textBrowser->clear();
      ui->brwoser->clear();
     statement *tmp=prolist->head;
    auto Map=prolist->exp_map;
     while (tmp->next) {
         tmp=tmp->next;
         ui->textBrowser->append(tmp->code);
        // qDebug()<<prolist->exp_map.size();
         ui->brwoser->append(Map[tmp->lineNum]->root->value);
     }
}
void MainWindow::set_syntree(){
    ui->brwoser->clear();

}
//载入流文件
void MainWindow::load(QTextStream &in){
    this->prolist->clear();

    QString str;
    while (!in.atEnd()) {

        str=in.readLine();
        statement *newline= new statement(str,nullptr);
         prolist->handleNew(newline);
    }
    set_result();
}

//输入完成后的函数
void MainWindow::on_lineEdit_returnPressed()
{
    QString str=ui->lineEdit->text();
     str=str.trimmed();
     if(str.length()==0) {
         ui->lineEdit->clear();
         return;
     }
     statement *newline= new statement(str,nullptr);

     if(prolist->handleNew(newline)==CmdStmt){

        if(newline->parts[0]=="RUN")
            {}//run_code();
        else if(newline->parts[0]=="HELP")
        {show_help();}//
        else if(newline->parts[0]=="CLEAR")
            {}
        else if(newline->parts[0]=="QUIT")
            this->close();
     }
     //新写入的一行不是cmd
      else {
         //三种特殊情况
        if(!newline->isfistNum){
            if(newline->kind==LetStmt){
                QString noNum="";
                for(int i=1;i<newline->parts.length();++i){
                    noNum+=newline->parts[i].trimmed()+" ";
                }//提取除LET外的其他字符
                noNum=noNum.trimmed();
               QList<QString>tmp_list=exp::get_token(noNum);
               qDebug()<<tmp_list;
               add_var(tmp_list[0],tmp_list[2].toInt());
            }
        }
        if(newline->kind==PrintStmt){
            QString noNum="";
            for(int i=1;i<newline->parts.length();++i){
                noNum+=newline->parts[i].trimmed()+" ";
            }//提取除行号外的其他字符
            noNum=noNum.trimmed();
           QList<QString>tmp_list=exp::get_token(noNum);
             ui->result->append(tmp_list[0]);
        }
     }
     cur_linenum++;

      ui->lineEdit->clear();
     set_result();//设置代码框

}

//load_button的槽函数--触发载入文件函数
void MainWindow::on_pushButton_3_clicked()
{

    QString filename=QFileDialog::getOpenFileName(this,tr("load"),".");
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    load(in);
    file.close();
}

void MainWindow::show_help(){
    QMessageBox tip(this);
    tip.setText("参考文档");
    tip.setWindowTitle("help");
    tip.exec();
}
void MainWindow:: add_var(QString name,int val){
    vals->add(name,val);
}
