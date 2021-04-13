#include "exp.h"
#include<QDebug>
#include<cmath>
exp::exp()
{

}
//用于判断元素是什么类型的
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
 QList<QString> exp::tokenizer(QString str){
QList<QString> list;
str=str.trimmed();

int len=str.length();
QString last;
int cur_num=0;
int cur_start=0;
for(int i=0;i<len;++i){

    if(str[i]=='+' || str[i]=='-'||str[i]=='/'||
            str[i]=='('||str[i]==')'||str[i]=='='||str[i]=='<'||str[i]=='>'){
        cur_num=i-1;
        if(cur_start<=cur_num){
            QString tmp=str.mid(cur_start,cur_num-cur_start+1);
            tmp=tmp.trimmed();
            list.push_back(tmp);

        }
        QString tmp=str[i]+"";
        list.push_back(tmp);
        cur_start=i+1;
        continue;
    }
    if(str[i]=='*'&&str[i+1]!='*'){
        //qDebug()<<"不是乘方";
        cur_num=i-1;
        if(cur_start<=cur_num){
            QString tmp=str.mid(cur_start,cur_num-cur_start+1);
            tmp=tmp.trimmed();
            list.push_back(tmp);
        }
        QString tmp=str[i]+"";
        list.push_back(tmp);
        cur_start=i+1;
         continue;
    }
    if(str[i]=='*'&&str[i+1]=='*'){
        cur_num=i-1;
        if(cur_start<=cur_num){
            QString tmp=str.mid(cur_start,cur_num-cur_start+1);
            tmp=tmp.trimmed();
            list.push_back(tmp);
        }
        QString tmp=str[i]+"";
        tmp+=(str[i+1]+"");
        list.push_back(tmp);
        cur_start=i+2;
        i++;
         continue;
    }

  if(i==len-1) {
      QString tmp=str.mid(cur_start);
      list.push_back(tmp);
  }

}
return list;
}

QList<QString> exp::get_token(QString str){
    QList<QString> tokenlist;
    QString tmp_str;
    str=str.trimmed();
    QList<QString> parts = str.split(' ',QString::SkipEmptyParts);
    int k=parts.size();
    for(int i=0;i<k;++i){
        QList<QString> tmp=tokenizer(parts[i]);

        while(!tmp.isEmpty()){
            tmp_str=tmp.front();
            tmp.pop_front();
            tokenlist.push_back(tmp_str);
        }

    }
   // tokenlist.pop_front();
    int len = tokenlist.length();
    QList<QString> tokens;
    for(int i=0;i<len;++i){
        if(tokenlist[i]=="-"){
            if(i==0) {
                tmp_str=tokenlist[0]+tokenlist[1];
                tokens.push_back(tmp_str);
                i++;
                continue;
            }
            if(tokenlist[i-1]=="("||tokenlist[i-1]=="="){
                tmp_str=tokenlist[i]+tokenlist[i+1];
                tokens.push_back(tmp_str);
                i++;
                continue;
            }
        }
        tmp_str=tokenlist[i];
       tokens.push_back(tmp_str);
    }
  //  qDebug()<<"提取出的tokens:"<<tokens;
    return tokens;
}
exp_node* exp::get_exp(QList<QString>oplist){

    bool isonly=false;
    int onekind=-2;
    if(oplist.size()==1) {
        onekind=isop(oplist.front());
        isonly=true;
    }
    QStack<QString> stack;//操作符栈
    QList<QString> postlist;
    QStack<exp_node* > numstack;//操作数栈
    QString tmp;

    while(!oplist.isEmpty()){

        tmp=oplist.front();//从表达式中取符号
        oplist.pop_front();
        int op_kind=isop(tmp);
        //qDebug()<<op_kind;
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
        else if(op_kind==-1||(op_kind>=1&&op_kind<3)){
            if(stack.isEmpty()){
                stack.push_back(tmp);
               // qDebug()<<tmp<<"进入stack";
            }
            else{
                QString top=stack.back();//取op栈的栈顶
                //如果栈顶的优先级大于或等于待入栈op，则弹出
                while (isop(top)>=isop(tmp)&&isop(top)!=4) {
                    stack.pop();//操作符栈弹出
                    if(numstack.isEmpty()) throw Error("表达式格式错误");
                    exp_node* r_node=numstack.back();
                    numstack.pop();
                    if(numstack.isEmpty())  throw Error("表达式格式错误");
                    exp_node* l_node=numstack.back();
                    numstack.pop();
                    exp_node* newnode = new exp_node(top,l_node,r_node);
                    newnode->kind=OPRAND;
                    numstack.push(newnode);
                    postlist.push_back(top);
                    top=stack.back();//重新设置栈顶
                }
                stack.push_back(tmp);
            }
        }
        else if(op_kind==3){
            if(stack.isEmpty()){
                stack.push_back(tmp);
               // qDebug()<<tmp<<"进入stack";
            }
            else{
                QString top=stack.back();//取op栈的栈顶
                //如果栈顶的优先级大于或等于待入栈op，则弹出
                while (isop(top)>isop(tmp)&&isop(top)!=4) {
                    stack.pop();//操作符栈弹出
                    if(numstack.isEmpty()) throw Error("表达式格式错误");
                    exp_node* r_node=numstack.back();
                    numstack.pop();
                    if(numstack.isEmpty()) throw Error("表达式格式错误");
                    exp_node* l_node=numstack.back();
                    numstack.pop();
                    exp_node* newnode = new exp_node(top,l_node,r_node);
                    newnode->kind=OPRAND;
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
                if(stack.isEmpty()) throw Error("表达式格式错误");
                if(numstack.isEmpty()) throw Error("表达式格式错误");
                exp_node* r_node=numstack.back();
                numstack.pop();
                if(numstack.isEmpty()) throw Error("表达式格式错误");
                exp_node* l_node=numstack.back();
                numstack.pop();
                exp_node* newnode = new exp_node(top,l_node,r_node);
                newnode->kind=OPRAND;
                numstack.push(newnode);//放回操作数栈
                postlist.push_back(top);
                top=stack.back();
            }
                stack.pop();//弹出左括号
        }
    }
   //读取完symbol后，将操作符栈中的运算符弹出栈
    while(!stack.isEmpty()){
            tmp=stack.back();
            stack.pop();
            if(numstack.isEmpty()) throw Error("表达式格式错误");
            exp_node* r_node=numstack.back();
            numstack.pop();
            if(numstack.isEmpty()) throw Error("表达式格式错误");
            exp_node* l_node=numstack.back();
            numstack.pop();
            exp_node* newnode = new exp_node(tmp,l_node,r_node);
            newnode->kind=OPRAND;
            numstack.push(newnode);

    }
    if(numstack.isEmpty()) throw Error("表达式格式错误");
    exp_node *tmp_node=numstack.back();
   numstack.pop();
   if(numstack.isEmpty()){
       tmp_node->kind=OPRAND;
      // qDebug()<<"成功创建exp_tree";
       if(isonly){
           bool isint;
           tmp_node->value.toInt(&isint);
           if(isint) tmp_node->kind=CONSTANT;
           else tmp_node->kind=VARIANT;
       }
       return tmp_node;
   }
   throw Error("表达式格式错误");//可根据这里报错
}
//输入一个除了行号外的string
exp::exp(QString str,bool flag){
    try {

         QList<QString> oplist=get_token(str);//将string 转化为多部份的一个string list

       //  qDebug()<<oplist;
         if(oplist.front()=="LET"||oplist.front()=="PRINT"){
            if(oplist.front()=="LET") tree_kind=LETexp;
            else tree_kind=PRINTexp;

            oplist.pop_front();//弹出语句类型

            if(tree_kind==LETexp){
             //  qDebug()<<"yes";
                 if(oplist.isEmpty()) throw Error("请输入表达式");
                 if(oplist.size()<3) throw Error("error");

                bool isnum;
                oplist.front().toInt(&isnum);
                if(isnum) throw Error("请勿给数值常量赋值");

              //  qDebug()<<oplist;

            }
            root=get_exp(oplist);//建立表达式树
            if(oplist.size()==1) {
                bool ok;
                oplist.front().toInt(&ok);
                if(ok)//是常数
                    root->kind=CONSTANT;
                else root->kind=VARIANT;


            }

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
             tree_kind=GOTOexp;
         }
         else if(oplist.front()=="INPUT"){
             oplist.pop_front();
             bool isnum;
             oplist.front().toInt(&isnum);
             if(isnum) throw Error("请勿给数值常量赋值");
             QString tmp=oplist.front();
             oplist.pop_front();
             if(oplist.isEmpty()){
                 exp_node *newnode=new exp_node(tmp);
                 newnode->kind=CONSTANT;
                 root=new exp_node("INPUT",newnode);
                 root->kind=DEFINE;
             }
             tree_kind=Inputexp;
         }
         else if (oplist.front()=="IF") {
             oplist.pop_front();
             QList<QString> exp1,exp2;
             exp_node *left,*right;
             exp_node *oprand,*then;
             QString tmp;
             while(!oplist.isEmpty()&&oplist.front()!="<"&&oplist.front()!=">"&&oplist.front()!="="){
                 tmp=oplist.front();
                 oplist.pop_front();
                 exp1.push_back(tmp);
             }
                 left=get_exp(exp1);
             //截取比较的符号是什么
             if(!oplist.isEmpty()){
                 oprand=new exp_node(oplist.front());
                 oprand->kind=OPRAND;
                 oplist.pop_front();
             }
             while(!oplist.isEmpty()&&oplist.front()!="THEN"){
                 tmp=oplist.front();
                 oplist.pop_front();
                 exp2.push_back(tmp);
             }
                 right=get_exp(exp2);
             if(oplist.front()=="THEN"){
                 oplist.pop_front();
             }
             tmp=oplist.front();
             then = new exp_node(tmp);
             then->kind=CONSTANT;
             root=new exp_node("IF THEN",left,right);
             root->kind=DEFINE;
             root->oprand=oprand;
             root->then=then;
             tree_kind=IFexp;
         }
         else {
            // qDebug()<<"yes";
             throw Error("UNKnown type");
         }
         return;

    }  catch (Error e) {
        if(flag) {throw e;}
        root=new exp_node("Error");
        tree_kind=Errorexp;
        return;

    }


}


int exp::get_node_value(exp_node* tmp,QMap<QString,int> all){
    if(tmp->kind==CONSTANT) return tmp->value.toInt();
    if(tmp->kind==VARIANT) {
       // qDebug()<<all[tmp->value];
        if(all.find(tmp->value)==all.end())//变量为定义
            throw Error("Variable "+tmp->value+" undifined");
        return all[tmp->value];
    }
    if(tmp->kind==DEFINE) throw Error("表达式树结构错误");
    if(tmp->kind==OPRAND) {
        int leftv=get_node_value(tmp->left,all);
        int rightv=get_node_value(tmp->right,all);
        if(tmp->value=="+") return leftv+rightv;
        if(tmp->value=="-") return leftv-rightv;
        if(tmp->value=="*") return leftv*rightv;
        if(tmp->value=="/") {
                if(rightv==0)
                    throw Error("divide 0");
            return leftv/rightv;
        }
        if(tmp->value=="**"){
            if(leftv==0&&rightv==0)
                throw Error("0的0次方");
            return pow(leftv,rightv);
        }

    }
}

int exp::calculate( QMap<QString,int> all){
    switch (tree_kind) {
    case LETexp:
        return get_node_value(root->right,all);
    case PRINTexp:
        return get_node_value(root,all);
    case IFexp:
    {
        int l=get_node_value(root->left,all);
        int r=get_node_value(root->right,all);
        if(root->oprand->value=="=") return int(l==r);
        else if (root->oprand->value==">") return int(l>r);
        else if (root->oprand->value=="<") return int(l<r);
    }
    case GOTOexp:
        throw Error("内部格式错误");
    }
}

void exp:: pre_order(QList<syn_node>& syns,exp_node*cur,int level){
            if(cur==nullptr) return;
            syn_node newnode={level,cur->value};
           // qDebug()<<newnode.str;
            syns.push_back(newnode);
            pre_order(syns,cur->left,level+1);
            pre_order(syns,cur->oprand,level+1);
            pre_order(syns,cur->right,level+1);
            pre_order(syns,cur->then,level+1);


}
 QList<syn_node> exp::get_pre_order(){
        QList<syn_node> syns;
        pre_order(syns,root);
        return syns;
}
 QList<QString> exp::get_syntax(){
     QList<syn_node> syns=get_pre_order();
     QList<QString> strs;
     while(!syns.isEmpty()){
         QString tmp=syns.front().str;
         for(int i=0;i<syns.front().level;++i){
             tmp="    "+tmp;   
         }
         strs.push_back(tmp);
         syns.pop_front();
     }
     if(this->tree_kind==LETexp){
         strs[0]="LET =";
     }
     if(this->tree_kind==PRINTexp){
         for(int i=0;i<strs.size();++i){
             strs[i]="    "+strs[i];
         }
         strs.push_front("PRINT");
     }
     return strs;
 }
