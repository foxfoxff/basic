#include "program.h"
#include<QDebug>
program::program()
{
    head=new statement;
    lineSum=0;
}

void program::clear(){
    statement *p = head->next;

    head->next = nullptr;
    while(p!=nullptr){
        statement *tmp = p->next;
        delete p;
        p=tmp;

    }
    lineSum=0;
}

void program::insert(QString newLine){
    statement *newline= new statement(newLine,nullptr);
    if(head->next==nullptr){
       if(newline->parts.length()!=1)
            {head->next=newline;lineSum++;}
    }
    else {
        statement *p = head;
        statement *q;
        while(p->next!=nullptr&&p->next->lineNum <= newline->lineNum){
            q=p;
            p=p->next;
        }

        //替换一行，p为要替代的一行
        if(p->lineNum==newline->lineNum){
            if(newline->parts.length()==1){
                   q->next=p->next;
                   delete p;
                   lineSum--;
            }
            else {
                p->code=newline->code;
            }

        }
        //增加一行，增加在p之后
        else{
            if(newline->parts.length()!=1){
                newline->next=p->next;
                p->next=newline;
                lineSum++;

            }

        }

    }

}
