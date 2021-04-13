#include "statement.h"
#include<QDebug>
statement::statement()
{
    next = nullptr;
}

//分析语句类型
void statement::choose_kind(QString part_k,bool hasnum){
    if(hasnum){
        if(part_k=="REM")
            kind=RemStmt;
        else if(part_k=="LET")
            kind=LetStmt;
        else if(part_k=="PRINT")
            kind=PrintStmt;
        else if(part_k=="INPUT")
            kind=IuputStmt;
        else if(part_k=="GOTO")
            kind=GotoStmt;
        else if(part_k=="IF")
            kind=IfStmt;
        else if(part_k=="END")
            kind=EndStmt;
         else kind=Unknown;
        return;
    }
    else{
        if(part_k=="LET")
            kind=LetStmt;
        else if(part_k=="PRINT")
            kind=PrintStmt;
        else if(part_k=="INPUT")
            kind=IuputStmt;
        else if(part_k=="RUN"||part_k=="HELP"||part_k=="CLEAR"||part_k=="LOAD") kind=CmdStmt;
        else kind=Unknown;
    }


}


//生成语句
statement::statement(QString value,statement*n){
    value=value.trimmed();
   parts = value.split(' ',QString::SkipEmptyParts);
   //qDebug()<<parts[0];

   parts[0].toInt(&isfistNum);
   code=value;
   //第一部分是数字
   if(isfistNum){
       lineNum=parts[0].toInt(&isfistNum);//设置行号
       if(lineNum>=10000||lineNum<0) throw Error("行号不符合规范");
       if(parts.size()==1) kind=DelStmt;//类型为del
       if(parts.size()>1){
           choose_kind(parts[1],true);
       }
   }
   //第一部分是字母--即是一个指令
   else{
       lineNum=0;
       choose_kind(parts[0],false);
   }

    next=n;
}
