#ifndef SINGLEVSSINGLE_H
#define SINGLEVSSINGLE_H
#include"SqliteDatabase.h"

class SingleVSsingle:public SqliteDatabase
{
public:
    bool _stop=true;

    SingleVSsingle();
    //重载必要的函数

    virtual void Inverse();
    virtual void dataRead1();
    virtual int calcScore();
    virtual void click(int id, int row, int col);
    void tranferStepRedFirst();//转化第一步棋谱
    void tranferStepRed(QString str);
    QString matchDateSVS();

public slots:
    virtual void computerMove();
};

#endif // SINGLEVSSINGLE_H
