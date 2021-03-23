#ifndef EXP_H
#define EXP_H
#include<QString>
#include<QList>
#include"tokenizer.h"
#include<QStack>

enum exp_t{CONSTANT,OPRAND,VARIANT,DEFINE};
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

    }
};

class exp
{
    friend class MainWindow;
    friend class program;

public:
    exp();
    exp(QString str);
    int isop(QString);
    static QList<QString>get_token(QString str);
    static QList<QString> tokenizer(QString str){
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
    exp_node* get_exp(QList<QString>);

private:
    exp_node *root;


};

#endif // EXP_H
