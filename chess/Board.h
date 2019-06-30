#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include <QVector>
#include<QPushButton>
#include <QPlainTextEdit>
class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent =NULL);
    ~Board();

    /* game data */
    Stone _s[32];
    int _r;
    QPoint _off;
    bool _bSide;
    int _turn=1;
//    int _turnForDelete;//未使用
    QPoint startPoint;
    QPoint endPoint;
    bool _PushKey=false;
    int _row11;
    int _col11;

    QPushButton *_back;//悔棋按钮;
    QPushButton *_draw;//求和按钮;
//    QPushButton *_check;//将军按钮;
    QPushButton *_saveChess;//保存按钮;
    QPushButton *_restoreChess;//复盘按钮;
    QVector<Step*> _steps;
    QVector<Step*> _stepsForRestore;
    int _stepsForRestoreCount;
    QPlainTextEdit * _plte=new QPlainTextEdit();
    //
    int _selectid;
    bool _bRedTurn;//执棋方
    void init(bool bRedSide);
    //辅助估值函数
    //enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
     int MobilityValues(int id);
    //划线
    void paintEvent(QPaintEvent *);
    void drawPlate(QPainter& p);
    void drawPlace(QPainter& p);
    void drawInitPosition(QPainter& p);
    void drawInitPosition(QPainter& p, int row, int col);
    void drawStone(QPainter& p);
    void drawStone(QPainter& p, int id);

    //辅助划线
    QPoint center(int row, int col);
    QPoint center(int id);
    QPoint topLeft(int row, int col);
    QPoint topLeft(int id);
    QRect cell(int row, int col);
    QRect cell(int id);

    bool getClickRowCol(QPoint pt, int& row, int& col);
//    bool getClickRowCol(QPoint pt, double& row, double& col)
    //棋子状态
    QString name(int id);
    bool red(int id);
    bool sameColor(int id1, int id2);
    int getStoneId(int row, int col);
    void killStone(int id);
    void reliveStone(int id);
    void moveStone(int moveid, int row, int col);
    bool isDead(int id);
    virtual void  getWin(int id);

    //走子
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);


    //规则判断
    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);

    bool canSelect(int id);

    //规则辅助
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
    int relation(int row1, int col1, int row, int col);
    bool isBottomSide(int id);

    //功能
    void inforText();
    void backButton();//悔棋按钮
    virtual void backStep();
    void turnColor();//提示下棋方
    virtual void draw();//求和
    void drawButton();
    void backOne();
    void back(Step* step);
    virtual void back();
    void saveChessButton();//保存按钮
    virtual void saveChess();//槽函数

    void restoreChessButton();//恢复按钮
    virtual void restoreChess();//槽函数
    void restoreGo(int ,QString );
   //辅助功能
    void informationShow(int moveid, int row, int col);//展示信息
    QString infor(bool isred,int col);//红方在下的配置方案
    void informationShowInver(int moveid, int row, int col);//展示信息
    QString inforInver(int ,int col);//黑方在下的配置方案
    void informationDelet();//撤销消息
    //键盘事件
    virtual void keyReleaseEvent(QKeyEvent *ev);

    virtual void checkmate();//将军
//    void checkanimation(int );

    void drawArrow(QPainter &p);//行走路径


signals:

public slots:
    void slotBack();
    //void slotDraw();

};

#endif // BOARD_H
