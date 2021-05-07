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
    ui->brwoser->append(QString::number(linenum)+" "+syntax_lines.front());
    syntax_lines.pop_front();
    while(!syntax_lines.isEmpty()){
        ui->brwoser->append(syntax_lines.front());
        syntax_lines.pop_front();

    }

}
void MainWindow::set_syntax(){
    statement *tmp=prolist->head;
    while(tmp->next){
        tmp=tmp->next;
        if(tmp->kind==RemStmt){
           // qDebug()<<"you are in REM";
                QString firstline=QString::number(tmp->lineNum)+" REM";
              //  qDebug()<<tmp->lineNum;
                //qDebug()<<firstline;
                ui->brwoser->append(firstline);
                  QString secondline="    ";
                 // qDebug()<<tmp->parts;
                  if(tmp->parts.length()>=3)
                  for(int i=2;i<tmp->parts.length();++i){
                      secondline+=tmp->parts[i].trimmed()+" ";
                    }
                  ui->brwoser->append(secondline);
        }
         if(tmp->kind==LetStmt||tmp->kind==PrintStmt||tmp->kind==IfStmt||tmp->kind==GotoStmt||tmp->kind==Unknown||tmp->kind==IuputStmt){
             exp* exp_sym=prolist->exp_map[tmp->lineNum];
             add_syntax(exp_sym,tmp->lineNum);
         }
         if(tmp->kind==EndStmt){
             ui->brwoser->append(QString::number(tmp->lineNum)+" END");
         }

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
void MainWindow::handleinput(QString val){
  // qDebug()<<"queue in handleinput:"<<vals->need_input;
    bool ok;
    int num=val.toInt(&ok);
   if(vals->need_input.isEmpty()) throw Error("没有待输入的变量");
   QString var=vals->need_input.front();
   //qDebug()<<var;
   vals->need_input.pop_front();
    if(ok){
        if(ifexist(var)) {
            //qDebug()<<"y";
            if(ifexist_str(var)) throw Error("conflict");
            vals->all[var]=num;
        }
        else {  if(ifexist_str(var)) throw Error("conflict"); add_var(var,num);}

    }
    else {

        if(ifexist_str(var)) {
            //qDebug()<<"y";
            if(ifexist(var)) throw Error("conflict");
            vals->all_str[var]=val;
        }
        else {  if(ifexist(var)) throw Error("conflict"); add_str(var,val);}

    }
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
    ui->textBrowser_2->clear();
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
            handleinput(tmp);
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

            if(newline->parts[0]=="RUN"){
                    ui->pushButton_2->clicked();
                    return;

            }//run_code();
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
            else if(newline->parts[0]=="LOAD")
               {
                ui->lineEdit->clear();
                ui->pushButton_3->click();
            }
         }
         //新写入的一行不是cmd
          else {
                    // 三种直接运行的特殊情况
                   if(!newline->isfistNum){
                       //LET
                        if(newline->kind==LetStmt){
                        exp *tmp = new exp(newline->code,true);
                        if(tmp->tree_kind==Errorexp) throw Error(" ERROR");
                        if(tmp->isStr()) {
                            if(ifexist(tmp->root->left->value)) throw Error("cf");

                            QString val = tmp->getstring();
                            if(ifexist_str(tmp->root->left->value)){
                                set_str(tmp->root->left->value,val);
                            }
                            else add_str(tmp->root->left->value,val);

                        }
                       else{
                            if(ifexist_str(tmp->root->left->value)) throw Error("cf");
                            int val=tmp->calculate(vals->all);
                            if(ifexist(tmp->root->left->value)) {

                                set_var(tmp->root->left->value,val);
                            }
                            else add_var(tmp->root->left->value,val);

                        }


                        }
                        //PRINT
                        if(newline->kind==PrintStmt){
                           qDebug()<<"success to enter the function";
                           // qDebug()<<vals->all["A"];
                            exp *tmp_exp = new exp(newline->code,true);
                            Print(tmp_exp);

                        }

                        //INPUT
                        if(newline->kind==IuputStmt){
                              QList<QString> oplist=exp::get_token(newline->code);
                              qDebug()<<oplist;
                              if(oplist.length()!=2) throw Error("语法错误");
                              //qDebug()<<newline->parts;
                              bool num;
                              oplist[1].toInt(&num);
                              if(num) throw Error("请勿给数值常量赋值");
                              vals->need_input.push_back(oplist[1]);
                              //qDebug()<<vals->need_input;
                              ui->lineEdit->setText("? ");
                              //qDebug()<<"进入了input";
                              return;
                        }
                        ui->lineEdit->clear();
                   }
                   ui->lineEdit->clear();

            }


         cur_linenum++;


         set_result();//设置代码框
    }  catch (Error a) {
       // qDebug()<<a.errname;
        QMessageBox tip(this);
        tip.setText(a.errname);
        tip.setWindowTitle("WRONG");
        tip.exec();
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
  // qDebug()<<"here";
    exp* tmp=prolist->exp_map[linenum];
   // add_syntax(tmp,linenum);
    if(tmp->tree_kind==Errorexp) throw Error(" ERROR");
    if(tmp->isStr()) {

        QString val = prolist->exp_map[linenum]->getstring();
        if(ifexist_str(tmp->root->left->value)){
            set_str(tmp->root->left->value,val);
        }
        else add_str(tmp->root->left->value,val);

    }
   else{
        int val=prolist->exp_map[linenum]->calculate(vals->all);
        if(ifexist(tmp->root->left->value)) {

            set_var(tmp->root->left->value,val);
        }
        else add_var(tmp->root->left->value,val);

    }

}
void MainWindow::Print(exp *tmp){
    if(tmp->tree_kind==Errorexp) throw Error(" ERROR");
   if(!tmp->isStr()){
       int val=tmp->calculate(vals->all);
     //  add_syntax(prolist->exp_map[linenum],linenum);
        ui->result->append(QString::number(val));
   }
   else {

       QString l =tmp->root->left->value;
       if(l[0]!='\"'&&l[0]!='\''){
           int _i=0,_s=0;
           _i=ifexist(l);
           _s=ifexist_str(l);
           if(_s&&_i) throw Error("conflict");
           if(_s) ui->result->append(vals->all_str[l]);
           if(_i) throw Error("conflict");
           if(!_i&&!_s) throw Error("no SUCH VARIABLE");
           return;
       }


       int index=0;
       int cur=0;
       int len = l.size();
       QString res="";
       QStringList rep = tmp->root->right->value.split(' ',QString::SkipEmptyParts);
       int rep_num=rep.size();
       while(rep_num){
           if(l[cur]=='{'&&l[cur+1]=='}'){
               l=l.mid(0,cur)+l.mid(cur+2,-1);
               if(rep[index][0]=='\''||rep[index][0]=='\"'){
                   l.insert(cur,rep[index].mid(1,rep[index].size()-2));
                   rep_num--;
                   cur+=(rep[index].size()-2);
                   index++;
               }
               else {
                   bool ok;
                   rep[index].toInt(&ok);
                   if(!ok) {
                       int a=0,b=0;
                       a=ifexist(rep[index]);
                       b=ifexist_str(rep[index]);
                       if(a&&b) throw Error("conflict");
                       if(a) rep[index]=QString::number(vals->all[rep[index]]);
                       if(b) rep[index]=vals->all_str[rep[index]].mid(1,vals->all_str[rep[index]].size()-2);
                   }
                   l.insert(cur,rep[index]);
                   rep_num--;
                   cur+=(rep[index].size());
                   index++;
               }

           }
           else{
               cur++;
           }

       }
        ui->result->append(l);



   }
}

//处理PRINT
void MainWindow::handlePrint(int linenum){
    exp* tmp=prolist->exp_map[linenum];
   // add_syntax(tmp,linenum);
    Print(tmp);

}
//处理IF
bool MainWindow::handleIF(int linenum){
    exp* tmp=prolist->exp_map[linenum];
   // add_syntax(tmp,linenum);
    if(tmp->tree_kind==Errorexp) throw Error(" ERROR");
    if(tmp->calculate(vals->all)){
        handleGoto(tmp->root->then->value.toInt());
        return true;
    }
    return false;
}




//code runner
statement* MainWindow::run_code(statement *cur_line){
    ui->lineEdit->clear();
    //cur_line的下一行开始执行
    try {
        while(cur_line->next){
            cur_line=cur_line->next;

            switch (cur_line->kind) {
            case RemStmt:
                   // qDebug()<<cur_line->lineNum;
                    break;
                case LetStmt:
                {
                handleLet(cur_line->lineNum);
              //  qDebug()<<cur_line->lineNum;
                break;
                }
                case PrintStmt:
                {
                handlePrint(cur_line->lineNum);
               // qDebug()<<cur_line->lineNum;
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

                  //  add_syntax(prolist->exp_map[cur_line->lineNum],cur_line->lineNum);

                    if(!isint) {
                        //qDebug()<<"here";
                        throw Error("第"+QString::number(cur_line->lineNum)+"行语法错误");
                    }
                    emit Goto(n);
                    return cur_line;
                }
                case IfStmt:{
                   //  add_syntax(prolist->exp_map[cur_line->lineNum],cur_line->lineNum);
                     if(handleIF(cur_line->lineNum)) return nullptr;

                    break;

                }
            case Unknown:{
                throw Error("未知语言");
            }

                 case EndStmt:
                    return nullptr;


            }

       }
        for(auto a=vals->all.begin();a!=vals->all.end();++a){
            qDebug()<<a.value();
             ui->textBrowser_2->append(a.key()+": INT ="+QString::number(a.value()));
        }
        for(auto a=vals->all_str.begin();a!=vals->all_str.end();++a){
            ui->textBrowser_2->append(a.key()+": STR ="+a.value());
        }
    }  catch (Error a) {
      //  qDebug()<<a.errname;
        QMessageBox tip(this);
        tip.setText(a.errname);
        tip.setWindowTitle("WRONG");
        tip.exec();
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
    tip.setText("\t\t\tHELP\n"
                "    This is a minimal Basic interpreter applied for integer and some of its statement.\n"
                "    You can edit code ended with a code line full with a linenumber and END in commandline window in the bottom of the mainwindow.\n"
                "    Now it supports statements and commands listed below:\n"
                "    INPUT,PRINT,LET,IF THEN,GOTO,REM,RUN,HELP,LOAD,END\n"
                "    Mention that LET,INPUT and PRINT can be recognized without a line number,it will be excuted immediately, and  will not be displayed in the form of syntax tree.\n"
                "    Another thing you need to remember is that any statement except command and three special statement listed above, will be ignored, which means it won't be displayed and won't have any effect to the"
                "program you typed in before.\n"
                "    To optimize your coding experience, the interpreter implement  real-time sort by line number, so you don't need to type in LIST command.\n"
                "    You can also load code file by type LOAD or click the button LOAD,it will open the file you choose and read the file line by line.\n"
                "    Type RUN to make your simple Basic program run,it will excuted your code in the order of the line number.\n"
                "    After you run the program, a series of syntax trees of the code you type in will be displayed in the syntax tree part.\n"
                "    Type QUIT or simply click the close button to exit.\n"
                "    To clear the window and variables you difined, you can just type CLEAR or click the button CLEAR.\n"
                "    Statement with syntax error will also be reminder in the syntax tree display part,you will see an Error flowing the linenumber.\n"
                "    Each time you type in code, the statement you type in should be case-sensitive.\n"
                "    For more information or detail about the syntax of BASIC, read the file BASIC-doc.pdf.\n"
                );
    tip.setWindowTitle("HELP");
    tip.exec();
}
void MainWindow:: add_var(QString name,int val){
    vals->add(name,val);
}
void MainWindow:: add_str(QString name,QString val){
    vals->add_str(name,val);
}
bool MainWindow:: ifexist(QString name){
    return vals->exist(name);
}
bool MainWindow:: ifexist_str(QString name){
    return vals->exist_str(name);
}

void MainWindow::on_pushButton_clicked()
{
    clear();
}
void MainWindow::run(){
    ui->brwoser->clear();
    ui->result->clear();
    ui->lineEdit->clear();
    QList<int> errpos=prolist->parse();
    set_syntax();
    run_code(prolist->head);
    QTextBrowser *code = ui->textBrowser;
    QTextCursor cursor(code->document());
    QList<QTextEdit::ExtraSelection> extras;
    QList<QPair<int, QColor>> highlights;
    for(int i=0;i<errpos.size();++i){
        highlights.push_back({errpos[i], QColor(255, 100, 100)});
    }

    qDebug()<<errpos;
    for (auto &line : highlights) {
     QTextEdit::ExtraSelection h;
     h.cursor = cursor;
     // 下面这些的功能，请大家自行查看文档
     h.cursor.setPosition(line.first);
     h.cursor.movePosition(QTextCursor::StartOfLine);
     h.cursor.movePosition(QTextCursor::EndOfLine);
     h.format.setProperty(QTextFormat::FullWidthSelection, true);
     h.format.setBackground(line.second);
     extras.append(h);
    }
    code->setExtraSelections(extras);
}
void MainWindow::on_pushButton_2_clicked()
{
   run();
}
void MainWindow::on_textBrowser_2_textChanged()
{

}
