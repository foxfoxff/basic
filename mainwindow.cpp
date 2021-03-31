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
    inputnum=0;
    //连接信号和槽
    connect(this, SIGNAL(Goto(int)), this, SLOT(handleGoto(int)));
    connect(this, SIGNAL(afterinput(statement *)), this, SLOT(continuerun(statement *)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_result(){
      ui->textBrowser->clear();
     // ui->brwoser->clear();
     statement *tmp=prolist->head;
    auto Map=prolist->exp_map;
     while (tmp->next) {
         tmp=tmp->next;
         ui->textBrowser->append(tmp->code);

         //ui->brwoser->append(Map[tmp->lineNum]->root->value);
     }
   //  ui->brwoser->append(tmp->code);
}
//添加语法树
void MainWindow:: add_syntax(exp *node,int linenum){
    QList<QString> syntax_lines=node->get_syntax();
    int a=0;
    while(!syntax_lines.isEmpty()){
        ui->brwoser->append(syntax_lines.front());
        syntax_lines.pop_front();

    }

}
//载入流文件
void MainWindow::load(QTextStream &in){


    QString str;
    while (!in.atEnd()) {

        str=in.readLine();
        statement *newline= new statement(str,nullptr);
         prolist->handleNew(newline);
    }
    set_result();
    //qDebug()<<prolist->head->lineNum;
}

//处理input
void MainWindow::handleinput(int num){
  // qDebug()<<"queue in handleinput:"<<vals->need_input;
   if(vals->need_input.isEmpty()) throw Error("没有待输入的变量");
   QString var=vals->need_input.front();
   //qDebug()<<var;
   vals->need_input.pop_front();
   if(ifexist(var)) {qDebug()<<"y";vals->all[var]=num;}
   else {  add_var(var,num);}
   if(!inputline.isEmpty()) {
       statement *tmp=inputline.front();
       inputline.pop_front();
       emit afterinput(tmp);
   }
}
//CLEAR
void MainWindow::clear(){
    ui->textBrowser->clear();
    ui->brwoser->clear();
    ui->result->clear();
    prolist->clear();
    vals->clear();
    inputline.clear();
    ui->lineEdit->clear();
    prolist=new program;
    vals=new evalstate;
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
                {run_code(prolist->head);}//run_code();
            else if(newline->parts[0]=="HELP")
            {ui->lineEdit->clear();show_help();}//
            else if(newline->parts[0]=="CLEAR")
                {ui->lineEdit->clear();
                clear();}
            else if(newline->parts[0]=="QUIT")
               {

                ui->lineEdit->clear();
                this->close();
            }
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
                        ui->lineEdit->clear();
                   }
                   ui->lineEdit->clear();

            }


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
//处理LET
void MainWindow::handleLet(int linenum){
    exp* tmp=prolist->exp_map[linenum];
    add_syntax(tmp,linenum);
    int val=prolist->exp_map[linenum]->calculate(vals->all);
    if(ifexist(tmp->root->left->value)) {

        set_var(tmp->root->left->value,val);
    }
    else add_var(tmp->root->left->value,val);

}

//处理PRINT
void MainWindow::handlePrint(int linenum){

    int val=prolist->exp_map[linenum]->calculate(vals->all);
    add_syntax(prolist->exp_map[linenum],linenum);
     ui->result->append(QString::number(val));
}
//处理IF
bool MainWindow::handleIF(int linenum){
    exp* tmp=prolist->exp_map[linenum];
    if(tmp->calculate(vals->all)){
        handleGoto(tmp->root->then->value.toInt());
        return true;
    }
    return false;
}

//展示syntax tree
/*void MainWindow:: add_syntax(exp* new_exp,int linenum){
    switch (new_exp->tree_kind) {
        case LetStmt:{
            QString num=QString::number(linenum)+" LET=";
            ui->brwoser->append(num);


       }


    }
}*/





//code runner
statement* MainWindow::run_code(statement *cur_line){
    ui->lineEdit->clear();
    //cur_line的下一行开始执行
    while(cur_line->next){
        cur_line=cur_line->next;

        switch (cur_line->kind) {
        case RemStmt:
                qDebug()<<cur_line->lineNum;
                break;
            case LetStmt:
            {
            handleLet(cur_line->lineNum);
            qDebug()<<cur_line->lineNum;
            break;
            }
            case PrintStmt:
            {
            handlePrint(cur_line->lineNum);
            qDebug()<<cur_line->lineNum;
            break;

         }

             case IuputStmt:
               {
                if(cur_line->parts.length()<=2) throw Error("第"+QString::number(cur_line->lineNum)+"行语法错误");
                vals->need_input.push_back(cur_line->parts[2]);
                ui->lineEdit->setText("? ");
                inputline.push_back(cur_line);
                return cur_line;
                }

              case GotoStmt:{
                bool isint;
                int n=cur_line->parts[2].toInt(&isint);

                add_syntax(prolist->exp_map[cur_line->lineNum],cur_line->lineNum);

                if(!isint) {qDebug()<<"here"; throw Error("第"+QString::number(cur_line->lineNum)+"行语法错误");}
                emit Goto(n);
                return cur_line;
            }
            case IfStmt:{
                 add_syntax(prolist->exp_map[cur_line->lineNum],cur_line->lineNum);
                 if(handleIF(cur_line->lineNum)) return nullptr;

                break;

            }

             case EndStmt:
                return nullptr;


        }

   }

}

void MainWindow::handleGoto(int linenum){

        statement *p= prolist->head;
        if(!p->next) throw Error("不存在第"+QString::number(linenum)+"行");
        while(p->next&&p->next->lineNum!=linenum){
            p=p->next;
        }
        if(!p->next) throw Error("不存在第"+QString::number(linenum)+"行");
        run_code(p);

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

void MainWindow::on_pushButton_clicked()
{
    clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    run_code(prolist->head);
}
