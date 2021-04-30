#include "evalstate.h"

evalstate::evalstate()
{
    num=0;
}

void evalstate::add(QString valname,int value){
    all.insert(valname,value);

}
void evalstate::add_str(QString valname,QString value){
    all_str.insert(valname,value);

}
void evalstate::clear(){
    all.clear();
    need_input.clear();
}
