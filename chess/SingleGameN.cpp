#include "SingleGameN.h"
#include <QTimer>
#include <QApplication>
#include <QTime>
#include<QDebug>
#include<QMessageBox>
//最大最小法-剪枝优化
SingleGameN::SingleGameN():Board()
{
    _level = 4;
    InverseButton();

}
void SingleGameN::Inverse()
{
    QMessageBox::StandardButton ret=QMessageBox::question(NULL,"WARNING","交换执棋方将强制重开本场比赛！");

    if(ret==QMessageBox::Yes)
    {
        inver=!inver;
        startPoint=center(-1,-1);
        endPoint=center(-1, -1);
        Board:: init(inver);        
        _plte->clear();
        if(_bRedTurn == inver)
            return;

        QTimer::singleShot(200, this, SLOT(computerMove()));
    }
}
void SingleGameN::InverseButton()
{
    _inver=new QPushButton("交换执棋");
    _inver->setFixedSize(_r*4,_r*2);
    _inver->setStyleSheet("background-color:blue;color:orange");

    _inver->move(_r*24+30,_r*17+5);
    _inver->setFont(QFont("Times", 20, QFont::Bold));

    _inver->show();
    _inver->setParent(this);
    connect(_inver,&QPushButton::clicked,this,&SingleGameN::Inverse);
}
void SingleGameN::click(int id, int row, int col)
{
    if(inver)
    {
        if(!this->_bRedTurn)
            return;

        Board::click(id, row, col);

        if(!this->_bRedTurn)
        {
            /* 启动0.1秒定时器，在0.1秒后电脑再思考 */
            QTimer::singleShot(100, this, SLOT(computerMove()));

        }
    }
    else {

        if(this->_bRedTurn)
            return;

       Board::click(id, row, col);

       if(this->_bRedTurn)
       {
           /* 启动0.1秒定时器，在0.1秒后电脑再思考 */
           QTimer::singleShot(100, this, SLOT(computerMove()));

       }

    }
}
void SingleGameN::computerMove()
{
    Step* step = getBestMove();
    if(_bSide)
    {
      informationShow(step->_moveid, step->_rowTo ,step->_colTo);
    }
    else {
       informationShowInver(step->_moveid, step->_rowTo ,step->_colTo);
    }
    startPoint=center(step->_moveid);
    endPoint=center(step->_rowTo, step->_colTo);
    _s[step->_moveid]._value+= MobilityValues(step->_moveid);
    moveStone(step->_moveid, step->_killid, step->_rowTo, step->_colTo);
    delete step;
    update();
    Board::getWin(step->_killid);

    QDateTime current_date_time =QDateTime::currentDateTime();
    if(_date_cu.isEmpty()&&_level<6)
    {
        _date_cu =current_date_time.toString("hhmm");
        qDebug() << _date_cu;
    }else {
        QString date=current_date_time.toString("hhmm");
        if(date.toInt()-_date_cu.toInt()>6)
        {

                _level++;

        }
        _date_cu =current_date_time.toString("hhmm");
    }
    qDebug()<<_date_cu;

}

void SingleGameN::getAllPossibleMove(QVector<Step *> &steps)
{

    int min=16, max=32;
    if(this->_bRedTurn)
    {
        min = 0;
        max = 16;
    }
    for(int i=min; i<max; ++i)
    {
        if(_s[i]._dead) continue;

        for(int row=0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                int killid = this->getStoneId(row, col);
                if(sameColor(killid, i)) continue;

                if(canMove(i, killid, row, col))
                {
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
    int Chong=_steps.size();//删除重复的移动
    if(Chong>=6)
    {
        for (int i = 0; i < steps.size(); i++)
        {
            if(steps.at(i)==_steps.at(Chong-2)&&steps.at(i)==_steps.at(Chong-4))//进行路径匹配
            {
                steps.erase(steps.begin()+i);//删除该行
            }
        }
    }
}

void SingleGameN::fakeMove(Step* step)
{
    killStone(step->_killid);
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

void SingleGameN::unfakeMove(Step* step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

/* 评价局面分 */
int SingleGameN::calcScore()
{
    int redTotalScore = 0;
    int blackTotalScore = 0;
    //enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    static int chessScore[] = {1000, 499, 501, 200, 15000, 100, 100};

    // 黑棋分的总数 - 红旗分的总数

    for(int i=0; i<16; ++i)//红
    {

        if(_s[i]._dead) continue;
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=25000;
        }
        redTotalScore+=(chessScore[_s[i]._type]+_s[i]._value+scoreSupport(i));
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=15000;
        }
    }

    for(int i=16; i<32; ++i)
    {
        if(_s[i]._dead) continue;
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=25000;
        }
        blackTotalScore +=(chessScore[_s[i]._type]+_s[i]._value+scoreSupport(i));
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=15000;
        }
    }
//    QTime time;
//    time= QTime::currentTime();
//    qsrand(time.msec()+time.second()*1000);
    if(inver)
    {
        return blackTotalScore - redTotalScore+rand()%5;
    }
    return redTotalScore-blackTotalScore+rand()%5;
}

int SingleGameN::getMaxScore(int level, int curMinScore)
{
    if(level == 0) return calcScore();

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);   // 是红旗的possiblemove

    int maxScore = -100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        if(step==NULL)
        {
            return maxScore;
        }
        fakeMove(step);
        int score = getMinScore(level-1, maxScore);
        unfakeMove(step);
        delete step;


        if(score >= curMinScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if(score > maxScore)
        {
            maxScore = score;
        }

    }
    return maxScore;
}

int SingleGameN::getMinScore(int level, int curMaxScore)
{


    if(level == 0) return calcScore();

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);   // 是红旗的possiblemove
    QDateTime current_date_time =QDateTime::currentDateTime();
    int minScore = 100000;
    if(_date_cu.isEmpty())
    {
        _date_cu =current_date_time.toString("hhmm");
        qDebug() << _date_cu;
    }else {
        QString date=current_date_time.toString("hhmm");
        if(date.toInt()-_date_cu.toInt()>2)
        {
            return minScore;
        }
    }



    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        if(step==NULL)
        {
            return minScore;
        }
        fakeMove(step);
        int score = getMaxScore(level-1, minScore);
        unfakeMove(step);
        delete step;

        if(score <= curMaxScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }

        if(score < minScore)
        {
            minScore = score;
        }

    }
    return minScore;
}

Step* SingleGameN::getBestMove()
{
    /*
        2.试着走一下
        3.评估走的结果
        4.取最好的结果作为参考
    */

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    // 2.试着走一下
    // 3.评估走的结果
    int maxScore = -100000;
    Step* ret = NULL;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();


        fakeMove(step);
        int score = getMinScore(_level-1, maxScore);
        unfakeMove(step);

        if(score > maxScore)
        {
            maxScore = score;
            if(ret) delete ret;
            ret = step;
        }
        else
        {
            delete step;
        }
    }

    // 4.取最好的结果作为参考
    return ret;
}

void SingleGameN::backStep()
{
    Board:: informationDelet();
    Board:: backOne();
    Board:: informationDelet();
    Board:: backOne();

}
int SingleGameN::scoreSupport(int id)
{
    //enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};

    //各子的控制区域价值（所在的位置的价值）
    int position=0;
    const int PositionValues[7][90] = { //0= col=0*10 row=9-row
      { // 车
        -6,  5, -2,  4,  8,  8,  6,  6,  6,  6,
         6,  8,  8,  9, 12, 11, 13,  8, 12,  8,
         4,  6,  4,  4, 12, 11, 13,  7,  9,  7,
        12, 12, 12, 12, 14, 14, 16, 14, 16, 13,
         0,  0, 12, 14, 15, 15, 16, 16, 33, 14,
        12, 12, 12, 12, 14, 14, 16, 14, 16, 13,
         4,  6,  4,  4, 12, 11, 13,  7,  9,  7,
         6,  8,  8,  9, 12, 11, 13,  8, 12,  8,
        -6,  5, -2,  4,  8,  8,  6,  6,  6,  6
      },
      { // 马
         0, -3,  5,  4,  2,  2,  5,  4,  2,  2,
        -3,  2,  4,  6, 10, 12, 20, 10,  8,  2,
         2,  4,  6, 10, 13, 11, 12, 11, 15,  2,
         0,  5,  7,  7, 14, 15, 19, 15,  9,  8,
         2,-10,  4, 10, 15, 16, 12, 11,  6,  2,
         0,  5,  7,  7, 14, 15, 19, 15,  9,  8,
         2,  4,  6, 10, 13, 11, 12, 11, 15,  2,
        -3,  2,  4,  6, 10, 12, 20, 10,  8,  2,
         0, -3,  5,  4,  2,  2,  5,  4,  2,  2
      },

      { // 炮
        0,  0,  1,  0, -1,  0,  0,  1,  2,  4,
        0,  1,  0,  0,  0,  0,  3,  1,  2,  4,
        1,  2,  4,  0,  3,  0,  3,  0,  0,  0,
        3,  2,  3,  0,  0,  0,  2, -5, -4, -5,
        3,  2,  5,  0,  4,  4,  4, -4, -7, -6,
        3,  2,  3,  0,  0,  0,  2, -5, -4, -5,
        1,  2,  4,  0,  3,  0,  3,  0,  0,  0,
        0,  1,  0,  0,  0,  0,  3,  1,  2,  4,
        0,  0,  1,  0, -1,  0,  0,  1,  2,  4
      },
      { // 兵
        0,  0,  0, -2,  3, 10, 20, 20, 20,  0,
        0,  0,  0,  0,  0, 18, 27, 30, 30,  0,
        0,  0,  0, -2,  4, 22, 30, 45, 50,  0,
        0,  0,  0,  0,  0, 35, 40, 55, 65,  2,
        0,  0,  0,  6,  7, 40, 42, 55, 70,  4,
        0,  0,  0,  0,  0, 35, 40, 55, 65,  2,
        0,  0,  0, -2,  4, 22, 30, 45, 50,  0,
        0,  0,  0,  0,  0, 18, 27, 30, 30,  0,
        0,  0,  0, -2,  3, 10, 20, 20, 20,  0
      }  ,
      { // 帅
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       1, -8, -9,  0,  0,  0,  0,  0,  0,  0,
       5, -8, -9,  0,  0,  0,  0,  0,  0,  0,
       1, -8, -9,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0
     },
     { // 士
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  3/-986,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0
     },
     { // 相
       0,  0, -2,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  3,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0, -2,  0,  0,  0,  0,  0,  0,  0
     },
    };
    //
//    int a =0;
//    for (int i=0;i<32;i++) {
//        if(_s[i]._dead)
//            a++;
//    }
    //判断棋子攻击和保护能力
    int b=0,c=0;
    for(int row=0; row<=9; ++row)
    {
        for(int col=0; col<=8; ++col)
        {
            int killid = this->getStoneId(row, col);
            if(killid==-1)continue;
            if(canMove(killid, id,_s[id]._row, _s[id]._col))
            {
                if(sameColor(killid, id))
                {
                    b++;//受到保护力
                }else{
                    c++;//被攻击力
                }

            }
        }
    }
    position=10*b-8*c;//计算位置的被攻击和被保护
    if(_bSide)
    {
//        col=0*10 row=9-row
         position +=PositionValues[_s[id]._type][_s[id]._col*10+abs(9-_s[id]._row)];
    }
    else {

        position +=PositionValues[_s[id]._type][abs(8-_s[id]._col)*10+abs(_s[id]._row)];
    }

    return position;
}

