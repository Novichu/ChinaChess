#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"
#include <QPlainTextEdit>
class SingleGameN : public Board
{
    Q_OBJECT
public:
    SingleGameN();

    virtual void click(int id, int row, int col);

    Step* getBestMove();

    //反转棋盘
    QPushButton *_inver;//反转按钮;
    bool inver=true;//控制反转;
    QString _date_cu;

    virtual void Inverse();
    void InverseButton();

    void getAllPossibleMove(QVector<Step*>& steps);

    void fakeMove(Step* step);
    void unfakeMove(Step* step);
    //评估函数

    int scoreSupport(int id);
    virtual int calcScore();

    int getMinScore(int level, int curMaxScore);
    int getMaxScore(int level, int curMinScore);

    int _level;
    virtual void backStep();


public slots:
    virtual void computerMove();
};

#endif // SINGLEGAME_H
