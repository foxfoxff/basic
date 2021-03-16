#include "statement.h"
#include<QDebug>
statement::statement()
{
    next = nullptr;
}
statement::statement(QString value,statement*n){
   parts = value.split(' ',QString::SkipEmptyParts);
   qDebug()<<parts[0];
   bool OK;
   parts[0].toInt(&OK);
   code=value;
   if(OK){
       lineNum=parts[0].toInt(&OK);
   }
   else{
       lineNum=0;
   }

    next=n;
}
