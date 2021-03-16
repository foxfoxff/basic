#ifndef PROGRAM_H
#define PROGRAM_H
#include"statement.h"

class program
{
    friend class MainWindow;
public:
    program();
    void clear();
    void insert(QString value);

private:
    int lineSum;
     //存储程序的链表
    statement *head;

};

#endif // PROGRAM_H
