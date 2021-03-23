#include "exp.h"

exp::exp()
{

}
//-1--"=",0--计算量, 1--"+ -",2--"* /",3--"**",4--左括号，5--"右括号"
int exp::isop(QString str){
    if(str=="+"||str=='-')
        return 1;
    if(str=="*"||str=="/")
        return 2;
    if(str=="**")
         return 3;
    if(str=="(")
        return 4;
    if(str==")")
        return 5;
    if(str=="=")
        return -1;
    else return 0;
}
QList<QString> exp::get_token(QString str){
    QList<QString> tokenlist;
    QString tmp_str;
    str=str.trimmed();
    QList<QString> parts = str.split(' ',QString::SkipEmptyParts);
    int k=parts.size();
    for(int i=1;i<k;++i){
        QList<QString> tmp=tokenizer(parts[i]);
        while(!tmp.isEmpty()){
            tmp_str=tmp.front();
            tmp.pop_front();
            tokenlist.push_back(tmp_str);
        }

    }
    tokenlist.pop_front();
    return tokenlist;
}
exp_node* exp::get_exp(QList<QString>oplist){
    oplist.pop_front();//弹出语句类型
    QStack<QString> stack;//操作符栈
    QList<QString> postlist;
    QStack<exp_node* > numstack;//操作数栈
    QString tmp;

    while(!oplist.isEmpty()){

        tmp=oplist.front();//从表达式中取符号

        oplist.pop_front();
        int op_kind=isop(tmp);

        //如果取出的是计算量
        if(op_kind==0){
            exp_node *newnode= new exp_node(tmp);
            bool ok;
            tmp.toInt(&ok);
            //是常数
            if(ok){
                newnode->kind=CONSTANT;
            }
            //是变量
            else newnode->kind=VARIANT;

            postlist.push_back(tmp);
            numstack.push(newnode);
        }

        //取出的是运算符
        else if(op_kind==-1||(op_kind>=1&&op_kind<=3)){
            if(stack.isEmpty()){
                stack.push_back(tmp);
            }
            else{
                QString top=stack.back();//取op栈的栈顶
                //如果栈顶的优先级大于或等于待入栈op，则弹出
                while (isop(top)>=isop(tmp)&&isop(top)!=4) {
                    stack.pop();//操作符栈弹出
                    exp_node* r_node=numstack.back();
                    numstack.pop();
                    exp_node* l_node=numstack.back();
                    numstack.pop();
                    exp_node* newnode = new exp_node(top,l_node,r_node);
                    numstack.push(newnode);
                    postlist.push_back(top);
                    top=stack.back();//重新设置栈顶
                }
                stack.push_back(tmp);
            }
        }
        //左括号
        else if(op_kind==4){
            stack.push_back(tmp);
        }
        //右括号
        else if(op_kind==5){
            QString top=stack.back();
            //此处以后可以加入语法规范检查
            while(isop(top)!=4){
                stack.pop();
                exp_node* r_node=numstack.back();
                numstack.pop();
                exp_node* l_node=numstack.back();
                numstack.pop();
                exp_node* newnode = new exp_node(top,l_node,r_node);
                numstack.push(newnode);//放回操作数栈
                postlist.push_back(top);
                top=stack.back();
            }
                stack.pop();
        }
    }
   exp_node *tmp_node=numstack.back();
   numstack.pop();
   if(numstack.isEmpty()){
       tmp_node->kind=OPRAND;
       return tmp_node;
   }
   return nullptr;//可根据这里报错
}
exp::exp(QString str){
    QList<QString> oplist=get_token(str);//将string 转化为多部份的一个string list
    if(oplist.front()=="LET"||oplist.front()=="PRINT"){
       root=get_exp(oplist);
    }
    else if(oplist.front()=="GOTO"){
        oplist.pop_front();
        QString tmp=oplist.front();
        oplist.pop_front();
        if(oplist.isEmpty()){
            exp_node *newnode=new exp_node(tmp);
            newnode->kind=CONSTANT;
            root=new exp_node("GOTO",newnode);
            root->kind=DEFINE;
        }
    }
    else if (oplist.front()=="IF") {
        oplist.pop_front();
        QList<QString>
        while(!oplist.isEmpty()&&oplist.front()!="<"&&oplist.front()!=">"&&oplist.front()!="="){
         exp_node* left=



        }
    }




}
