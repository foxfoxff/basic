#ifndef PROGRAM_H
#define PROGRAM_H
#include"statement.h"
#include "exp.h"
#include<QMap>
enum Command{HELP,RUN,LET,LIST,CLEAR,QUIT};
class program
{
    friend class MainWindow;
    friend class exp;
    friend class statement;
public:
    program();
    void clear();
    state_t handleNew(statement *newline);//根据返回值可判断新的语句类型
   // Command insert(QString value);//将新语句加入链表
    void runcode();
    void showhelp();
private:
    //语句总数
    int lineSum;
    //存储程序的链表
    statement *head;
    //存储表达式树的链表
    QMap<int,exp*> exp_map;




};

#endif // PROGRAM_H
