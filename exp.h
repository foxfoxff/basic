#ifndef EXP_H
#define EXP_H
#include<QString>
#include<QList>
#include<QStack>
#include<error.h>

enum exp_t{CONSTANT,OPRAND,VARIANT,DEFINE};
enum exp_kind{LETexp,IFexp,GOTOexp,PRINTexp,Errorexp,Inputexp};
struct exp_node{
    exp_t kind;
    QString value;
    exp_node *left;
    exp_node *right;
    exp_node *oprand;//only for IF
    exp_node *then;//only for IF
    exp_node(QString val,exp_node* l=nullptr,exp_node*r=nullptr){
        value=val;
        left=l;
        right=r;
        oprand=nullptr;
        then=nullptr;

    }
};
struct syn_node{
    int level;
    QString str;
};
class exp
{
    friend class MainWindow;
    friend class program;

public:
    exp();
    exp(QString str,bool a=false);
    int isop(QString);
    static QList<QString>get_token(QString str);
    static QList<QString> tokenizer(QString str);
    exp_node* get_exp(QList<QString>);
    int calculate( QMap<QString,int> all);
    int get_node_value(exp_node*,QMap<QString,int> all);
    QList<QString> get_syntax();//获得某条语句对应的要展示的语法树
    QList<syn_node> get_pre_order();//获得前序遍历的结果
    void pre_order(QList<syn_node>&t,exp_node*,int level=0);//前序遍历

private:
    exp_node *root;
    exp_kind tree_kind;





};

#endif // EXP_H
