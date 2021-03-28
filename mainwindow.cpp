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

     //将新的语句加入链表，创建相应的exp_tree存入map中，并返回新语句的类型
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



                // 三种直接运行的特殊情况
               if(!newline->isfistNum){
                   //LET
                    if(newline->kind==LetStmt){
                   exp *tmp_exp = new exp(newline->code);
                   int val=tmp_exp->calculate(vals->all);
                   QList<QString> tokenlist= exp::get_token(newline->code);

                   if(ifexist(tokenlist[1]))
                       vals->all[tokenlist[1]]=val;
                   else add_var(tokenlist[1],val);
                    }
                    //PRINT
                    if(newline->kind==PrintStmt){
                        qDebug()<<"success to enter the function";
                        qDebug()<<vals->all["A"];
                        exp *tmp_exp = new exp(newline->code);
                        int val=tmp_exp->calculate(vals->all);



                         ui->result->append(QString::number(val));
                    }
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

bool MainWindow:: ifexist(QString name){
    return vals->exist(name);
}
