#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();
    //移动棋子的ID
    //吃掉棋子的ID，悔棋时需要将其_dead属性改变
    //起先的行
    //起先的列
    //现在的行
    //现在的列

    int _moveid;
    int _killid;
    int _rowFrom;
    int _colFrom;
    int _rowTo;
    int _colTo;
    //bool _redTurnPre;//上一步执棋方

signals:

public slots:
};

#endif // STEP_H
