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
     statement *tmp=prolist->head;
     while (tmp->next) {
         tmp=tmp->next;
         ui->textBrowser->append(tmp->code);
     }


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
      else {

     }
     cur_linenum++;

      ui->lineEdit->clear();
     set_result();
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
