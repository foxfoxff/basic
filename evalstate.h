#ifndef EVALSTATE_H
#define EVALSTATE_H

#include<QMap>
#include<QQueue>
class evalstate
{
 friend class program;
 friend class MainWindow;

public:
    evalstate();
    void add(QString,int);
    void clear();
    void add_str(QString valname,QString value);
    bool exist(QString name){
        auto p = all.find(name);
        if(p==all.end()) return false;
        else return true;

    }
private:
    int num;
    QMap<QString,int> all;
    QMap<QString,QString> all_str;
    QQueue<QString> need_input;
};

#endif // EVALSTATE_H
