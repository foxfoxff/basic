#ifndef STATEMENT_H
#define STATEMENT_H
#include<QString>
#include<QStringList>

class statement
{
    friend class program;
    friend class mainwindow;
private:


    QStringList parts;
public:
    statement();
    statement(QString state,statement*n = nullptr);
    QString code;
    statement *next;
    int lineNum;
};

#endif // STATEMENT_H
