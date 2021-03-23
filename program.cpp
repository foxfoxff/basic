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



//判断插入还是执行操作
state_t program::handleNew(statement *newline){

    if(newline->kind==CmdStmt){
        return CmdStmt;
    }
    //插入或删除节点
    else{
        if(!newline->isfistNum&&newline->kind!=DelStmt){
             return newline->kind;
        }
        if(head->next==nullptr){
           if(newline->parts.length()!=1)
                {head->next=newline;
               lineSum++;
               if(newline->kind==LetStmt&&newline->lineNum>0){

                   // exp_node*new_exp=new exp_node(newline->parts[2])
                    //exp_map.insert(newline->lineNum,)
               }

           }
        }
        else {
            statement *p = head;
            statement *q;
            while(p->next!=nullptr&&p->next->lineNum <= newline->lineNum){
                q=p;
                p=p->next;
            }

            //删除一行，p为要删除的一行
            if(p->lineNum==newline->lineNum){
                if(newline->kind==DelStmt){
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
    return newline->kind;

}
