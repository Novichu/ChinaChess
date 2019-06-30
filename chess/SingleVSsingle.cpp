#include "SingleVSsingle.h"
#include"SqliteDatabase.h"
#include <QTimer>
#include <QApplication>
#include <QTime>
#include<QMessageBox>
SingleVSsingle::SingleVSsingle()
{
    _inver->setText("开始");
    _draw->setText("重新开始");


}
void SingleVSsingle::click(int id, int row, int col)
{
    return;
}
void SingleVSsingle::computerMove()
{
//    int Chong=_steps.size();
//    if(Chong>=6&&Chong%2==0)
//    {
//        if(_steps.at(Chong)==_steps.at(Chong-2)&&_steps.at(Chong)==_steps.at(Chong-2)&&_steps.at(Chong-2)==_steps.at(Chong-4))
//        {

//            if(_s[_steps.at(Chong)->_moveid]._type==Stone::JIANG)
//            {
//                if(_s[_steps.at(Chong)->_moveid]._red)
//                {
//                    QMessageBox::information(NULL,"结束提示","黑色方胜利");

//                }
//                else {

//                     QMessageBox::information(NULL,"结束提示","红色方胜利");
//                }

//            }else {
//                QMessageBox::information(NULL,"结束提示","双方和棋");
//            }
//        }
//    }

    QString data=NULL;
    if(!end){
        data=dataQuery();
    }

    if(data==NULL)
    {
        qDebug() << "脱离数据库";
        SingleGameN::computerMove();
    }
    else {
        qDebug() << "我开始走了";
        if(!_bRedTurn)
        {
            tranferStep(data);
        }
        else {
            tranferStepRed(data);
            qDebug() << "尝试走红色棋谱";
        }

        informationShow(_s_s[0],_s_s[2] ,_s_s[3]);
        startPoint=center(_s_s[0]);
        endPoint=center(_s_s[2], _s_s[3]);
        _s[_s_s[0]]._value+= MobilityValues(_s_s[0]);
        moveStone(_s_s[0], _s_s[1], _s_s[2], _s_s[3]);

        qDebug() << "走完了";
        update();
    }


}
int SingleVSsingle::calcScore()

{
    int redTotalScore = 0;
    int blackTotalScore = 0;
    //enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    static int chessScore[] = {1000, 499, 501, 200, 150000, 100, 100};

    // 黑棋分的总数 - 红旗分的总数

    for(int i=0; i<16; ++i)//红
    {

        if(_s[i]._dead) continue;
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=250000;
        }
        redTotalScore += (chessScore[_s[i]._type]+_s[i]._value+scoreSupport(i));
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=150000;
        }
    }

    for(int i=16; i<32; ++i)
    {
        if(_s[i]._dead) continue;
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=250000;
        }
        blackTotalScore += (chessScore[_s[i]._type]+_s[i]._value+scoreSupport(i));
        if(_s[i]._type==Stone::JIANG)
        {
            chessScore[_s[i]._type]=150000;
        }
    }
//    QTime time;
//    time= QTime::currentTime();
//    qsrand(time.msec()+time.second()*1000);
    if(!_bRedTurn)//黑棋子走动
    {
        return blackTotalScore - redTotalScore+rand()%5;
    }
    return redTotalScore-blackTotalScore+rand()%5;//红棋子走动
}
void SingleVSsingle::dataRead1()
{

    dataRead();
    QString temp=matchDateSVS();
    if (!end && temp==NULL)
    {
        bool thennext=sql_query.next();

          if(!thennext)
          {
            qDebug() << "数据库结束";
            end= true;
          }
        dataRead1();
        qDebug() << "没有找到路径";

    }
    else {
        _s_step=temp;
        qDebug() << "找到路径";
    }
}

void SingleVSsingle::Inverse()
{

//    _inver->setDisabled(true);
//    _inver->setStyleSheet("background-color:gray;color:orange");
    _stop=!_stop;
    if (Board:: _steps.size() == 0)
    {
            tranferStepRedFirst();
    }
    QTimer *timer = new QTimer(this);
   if(!_stop)
    {
          connect(timer, SIGNAL(timeout()), this, SLOT(stop()));
    }

   connect(timer, SIGNAL(timeout()), this, SLOT(computerMove()));
   timer->start(1000);



}

void SingleVSsingle::tranferStepRedFirst()
{
    QString select_all_sql = "SELECT * FROM chess ORDER BY random() LIMIT 1";

    qDebug() <<select_all_sql;
    sql_query=QSqlQuery(database);

    sql_query.prepare(select_all_sql);
    sql_query.exec();
    sql_query.first();
    _s_chess[0] = sql_query.value(0).toString().trimmed();

    qDebug()<<"随机开局步子"+_s_chess[0];


//    _s_chess[0]="马二进三";
    tranferStepRed(_s_chess[0]);

    //_s_s  id killid row col
    //    this->_row = 9-this->_row;
    //    this->_col = 8-this->_col;

//    _s[_s_s[0]]._row=abs(9-_s[_s_s[0]]._row);
//    _s[_s_s[0]]._col=abs(8-_s[_s_s[0]]._col);

//    _s_s[2]=abs(9- _s_s[2]);
//    _s_s[3]=abs(8- _s_s[2]);

    informationShow(_s_s[0],_s_s[2] ,_s_s[3]);
    startPoint=center(_s_s[0]);
    endPoint=center(_s_s[2], _s_s[3]);
    moveStone(_s_s[0], _s_s[1], _s_s[2], _s_s[3]);
    sql_query.first();
}
QString SingleVSsingle::matchDateSVS()
{
    qDebug() << "我在查找哦";
    QString str=NULL,str2=NULL;

    if(_pace>=10)
    {return str2;}

    for (int i=0;i<_pace;++i) {
        if(i>=10)
        {return str2;}
        str=str+_s_chess[i];
         qDebug() <<str;
         qDebug() << "我在累加哦";
    }
    QString plte=_plte->document()->toPlainText().trimmed();
//    qDebug()<<plte;
    for (int i=1;i<_turn+1;i++) {
        plte.replace("第"+QString::number(i,10)+"回合","").replace("\n","").remove(QRegExp("\\:")).remove(QRegExp("\\s+"));
//        qDebug()<<plte+"----------------------------------------";
    }
    str.replace("车","車").replace("马","馬");
    str.replace("１","1").replace("２","2").replace("３","3").replace("４","4").replace("５","5").replace("６","6").replace("７","7").replace("８","8").replace("９","9");

    if(str==plte)
    {
        str2=_s_chess[_pace];
        qDebug() << "查找到了哦"<<str2;
        _pace+=1;
    }

    qDebug() << "查找结束哦";
    return str2;
}
void SingleVSsingle::tranferStepRed(QString str)
{
    QString c[4];
    qDebug() <<"开始转换";

    for (int i=0;i<str.length();i++) {
       c[i]=str.mid(i,1);
           qDebug() <<c[i];
    }

    Stone::TYPE d;

    int b1,b2;
    if(c[0]=="车") d= Stone:: CHE  ;
    if(c[0]=="马") d= Stone::MA   ;
    if(c[0]=="炮") d= Stone::PAO  ;
    if(c[0]=="兵") d= Stone::BING ;
    if(c[0]=="帅") d= Stone::JIANG;
    if(c[0]=="仕") d= Stone::SHI  ;
    if(c[0]=="相") d= Stone::XIANG;

    if(c[1]=="一") b1=8 ;
    if(c[1]=="二") b1=7 ;
    if(c[1]=="三") b1=6 ;
    if(c[1]=="四") b1=5 ;
    if(c[1]=="五") b1=4 ;
    if(c[1]=="六") b1=3 ;
    if(c[1]=="七") b1=2 ;
    if(c[1]=="八") b1=1 ;
    if(c[1]=="九") b1=0 ;

    if(c[3]=="一") b2=1 ;
    if(c[3]=="二") b2=2 ;
    if(c[3]=="三") b2=3 ;
    if(c[3]=="四") b2=4 ;
    if(c[3]=="五") b2=5 ;
    if(c[3]=="六") b2=6 ;
    if(c[3]=="七") b2=7 ;
    if(c[3]=="八") b2=8 ;
    if(c[3]=="九") b2=9 ;

    qDebug() << "字符串拆解完毕";
    qDebug() <<b1<<b2;
    int min=0, max=16;

    for(int i=min; i<max; ++i)
    {
        if(_s[i]._dead) continue;

        for(int row=0; row<=9; ++row)
        {
            //qDebug() << "尝试找到棋子";
            if(_s[i]._row == row &&_s[i]._col == b1 && _s[i]._type == d)
            {
                qDebug() << "找到棋子";

                _s_s[0]=i;

                qDebug() << "记录初始点";
                if(_s[i]._type==Stone::CHE||_s[i]._type==Stone::PAO||_s[i]._type==Stone::BING||_s[i]._type==Stone::JIANG)
                {
                    if(c[2]=="进")
                    {
                        _s_s[2]= _s[i]._row-b2;
                        _s_s[3]= _s[i]._col;
                        qDebug() <<"进";
                    }
                    if(c[2]=="退")
                    {
                        _s_s[2]= _s[i]._row+b2;
                        _s_s[3]= _s[i]._col;
                        qDebug() <<"退";
                    }
                    if(c[2]=="平")
                    {
                        _s_s[2]= _s[i]._row;
                        _s_s[3]=abs(8-(b2-1)) ;
                         qDebug() <<"平";
                    }
                }else
                 {
                    if(_s[i]._type==Stone::MA)
                    {
                        if(c[2]=="进")
                        {
                            _s_s[3]=abs(_s[i]._col-(8-(b2-1))) ;
                            if(_s_s[3]==1)
                            {
                                _s_s[2]= _s[i]._row-2;
                            }
                            else {
                                _s_s[2]= _s[i]._row-1;
                            }
                            _s_s[3]=abs(8-(b2-1));
                           qDebug() <<"进";
                        }
                        if(c[2]=="退")
                        {
                            _s_s[3]= abs(_s[i]._col-(8-(b2-1)));
                            if(_s_s[3]==1)
                            {
                                _s_s[2]= _s[i]._row+2;
                            }
                            else {
                                _s_s[2]= _s[i]._row+1;
                            }
                            qDebug() <<"退";
                            _s_s[3]=abs(8-(b2-1));
                        }
                    }
                   if(_s[i]._type==Stone::XIANG)
                   {
                       if(c[2]=="进")
                       {

                           _s_s[2]= _s[i]._row-2;

                           _s_s[3]=abs(8-(b2-1));
                          qDebug() <<"进";
                       }
                       if(c[2]=="退")
                       {
                           _s_s[2]= _s[i]._row+2;

                           _s_s[3]=abs(8-(b2-1));
                           qDebug() <<"退";
                       }
                   }
                   if(_s[i]._type==Stone::SHI)
                   {
                       if(c[2]=="进")
                       {

                           _s_s[2]= _s[i]._row-1;

                           _s_s[3]=abs(8-(b2-1));
                          qDebug() <<"进";
                       }
                       if(c[2]=="退")
                       {
                           _s_s[2]= _s[i]._row+1;

                           _s_s[3]=abs(8-(b2-1));
                           qDebug() <<"退";
                       }
                   }
                }

                _s_s[1]=getStoneId(_s_s[2],_s_s[3]);


            }
        }
    }
}
