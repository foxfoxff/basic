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
    exp_map.clear();
}



//判断插入还是执行操作
state_t program::handleNew(statement *newline){
    /*if(newline->kind==DelStmt){
        auto p=exp_map.find(newline->lineNum);
        if(p==exp_map.end()) throw Error("该行不存在");
    }*/


    if(newline->kind==CmdStmt){
        return CmdStmt;
    }

    else{
        //三种特殊情况 无须存储
        if(!newline->isfistNum&&newline->kind!=DelStmt){
             return newline->kind;
        }
         //插入或删除节点
        if(head->next==nullptr){
           if(newline->parts.length()!=1&&newline->isfistNum)
                {

                /*QString noNum="";
                for(int i=1;i<newline->parts.length();++i){
                    noNum+=newline->parts[i].trimmed()+" ";
                }//提取除行号外的其他字符
                noNum=noNum.trimmed();               
                 exp* new_exp= new exp(noNum);                
                 exp_map.insert(newline->lineNum,new_exp);*/
                 head->next=newline;
                 lineSum++;

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
                       /*exp_map.remove(newline->lineNum);*/
                }
                else {
                    throw Error("第"+QString::number(newline->lineNum)+"行存在，请先删除");
                }
            }
            //增加一行，增加在p之后
            else{
                if(newline->parts.length()!=1){

                   /* QString noNum="";
                    for(int i=1;i<newline->parts.length();++i){
                        noNum+=newline->parts[i].trimmed()+" ";
                    }//提取除行号外的其他字符
                     exp* new_exp= new exp(noNum);
                     exp_map.insert(newline->lineNum,new_exp);*/
                     newline->next=p->next;
                     p->next=newline;
                     lineSum++;
                }
            }

        }

    }
    return newline->kind;

}


bool program::parseline(statement* newline){


    try {
        QString noNum="";
        for(int i=1;i<newline->parts.length();++i){
            noNum+=newline->parts[i].trimmed()+" ";
        }//提取除行号外的其他字符
        noNum=noNum.trimmed();

         exp* new_exp= new exp(noNum);
       //  qDebug()<<noNum;
         exp_map.insert(newline->lineNum,new_exp);
         return true;
    }  catch (Error) {
        return false;
    }


}

QList<int> program::parse(){
    statement *cur_line=head->next;
    QList<int> errnos;
    while(cur_line){
       // qDebug()<<"parse"+QString::number(cur_line->lineNum);
        if(!parseline(cur_line))
            errnos.push_back(cur_line->lineNum);
          cur_line=cur_line->next;
    }
    return errnos;
}
