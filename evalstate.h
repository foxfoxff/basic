#ifndef EVALSTATE_H
#define EVALSTATE_H

#include<QMap>

class evalstate
{
 friend class program;
 friend class MainWindow;

public:
    evalstate();
    void add(QString,int);
    void clear();
    bool exist(QString name){
        auto p = all.find(name);
        if(p==all.end()) return false;
        else return true;

    }
private:
    int num;
    QMap<QString,int> all;
};

#endif // EVALSTATE_H
