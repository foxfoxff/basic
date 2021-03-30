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
    //ui->lineEdit->setText("?");
    inputnum=0;

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
         //ui->brwoser->append(Map[tmp->lineNum]->root->value);
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

//处理input
void MainWindow::handleinput(int num){
  // qDebug()<<"queue in handleinput:"<<vals->need_input;
   if(vals->need_input.isEmpty()) throw Error("没有待输入的变量");
   QString var=vals->need_input.front();
   //qDebug()<<var;
   vals->need_input.pop_front();
   if(ifexist(var)) {qDebug()<<"y";vals->all[var]=num;}
   else { qDebug()<<"n";   add_var(var,num);}
}

//输入完成后的函数
void MainWindow::on_lineEdit_returnPressed()
{
    try {
        QString str=ui->lineEdit->text();
        if(str[0]=='?') {
            QList<QString> a=str.split(' ',QString::SkipEmptyParts);
            QString tmp=a[1].trimmed();
            QString sym=a[0].trimmed();
            if(sym!="?") throw Error("语法错误");
            if(a.length()<2) throw Error("请输入值");
            //qDebug()<<tmp;
            handleinput(tmp.toInt());
            ui->lineEdit->clear();
            return;
        }

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
                       qDebug()<<"计算成功"<<val;
                       QList<QString> tokenlist= exp::get_token(newline->code);
                        //qDebug()<<tokenlist;
                       if(ifexist(tokenlist[1]))
                          {
                            vals->all[tokenlist[1]]=val;

                       }
                       else add_var(tokenlist[1],val);
                        }
                        //PRINT
                        if(newline->kind==PrintStmt){
                           qDebug()<<"success to enter the function";
                           // qDebug()<<vals->all["A"];
                            exp *tmp_exp = new exp(newline->code);
                            int val=tmp_exp->calculate(vals->all);

                             ui->result->append(QString::number(val));

                        }
                        //INPUT
                        if(newline->kind==IuputStmt){
                              if(newline->parts.length()<2) throw Error("语法错误");
                              //qDebug()<<newline->parts;
                              vals->need_input.push_back(newline->parts[1]);
                              //qDebug()<<vals->need_input;
                              ui->lineEdit->setText("? ");
                              qDebug()<<"进入了input";
                              return;
                        }
                   }

            }

         ui->lineEdit->clear();
         cur_linenum++;


         set_result();//设置代码框
    }  catch (Error a) {
        qDebug()<<a.errname;

        }


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
