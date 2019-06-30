#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <qdebug.h>
#include <QPlainTextEdit>
#include<SingleGameN.h>
class SqliteDatabase:public SingleGameN
{
public:


    SqliteDatabase();
    int _s_s[4];//id   killedID row col
    int _pace=1;

    QString _s_step=NULL;
    QString _s_chess[10];
    QSqlDatabase database;
    QSqlQuery sql_query;
    //查询数据库
    QString dataQuery();
    void dataRead();
    virtual void dataRead1();
    QString matchDate();
    bool end=false;
    void aiRedGo();//电脑走第一步的
    void tranferStep(QString str);//转化棋谱

    virtual void Inverse();//翻转
    virtual void backStep();
    virtual void  getWin(int id);
        virtual void draw();
public slots:
    virtual void computerMove();

};

#endif // SQLITEDATABASE_H
