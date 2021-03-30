#ifndef STATEMENT_H
#define STATEMENT_H
#include<QString>
#include<QStringList>
#include"error.h"
//statement kinds
enum state_t{RemStmt,LetStmt,PrintStmt,IuputStmt,GotoStmt,IfStmt,EndStmt,CmdStmt,DelStmt};

class statement
{
    friend class program;
    friend class MainWindow;
    friend class parser;
private:
    QString code;//原始代码
    statement *next;
    int lineNum;//行号
    state_t kind;
    bool isfistNum;
    QStringList parts;
public:
    statement();
    statement(QString state,statement*n = nullptr);

    void  choose_kind(QString kind);


};

#endif // STATEMENT_H
