#include "SqliteDatabase.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <qdebug.h>
#include <QTimer>
#include<QTime>
#include<QMessageBox>
SqliteDatabase::SqliteDatabase()
{
    //打开数据库

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {

        database = QSqlDatabase::database("qt_sql_default_connection");
        qDebug()<< "Table Connect" ;


    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("chess.db");
        database.setUserName("chess");
        database.setPassword("chess");
        qDebug()<< "Table Created" ;
    }

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        qDebug() << "open sucess";
    }

}
void SqliteDatabase::computerMove()
{
    QString data=NULL;
    if(!end){
        data=dataQuery();
    }
    if (_bSide)
    {
        if(data==NULL)
        {
            qDebug() << "脱离数据库";
            SingleGameN::computerMove();
        }
        else {
            qDebug() << "我开始走了";

            tranferStep(data);
            informationShow(_s_s[0],_s_s[2] ,_s_s[3]);
            startPoint=center(_s_s[0]);
            endPoint=center(_s_s[2], _s_s[3]);
            _s[_s_s[0]]._value+= MobilityValues(_s_s[0]);
            moveStone(_s_s[0], _s_s[1], _s_s[2], _s_s[3]);

            qDebug() << "走完了";
            update();
            Board::getWin(_s_s[1]);
        }
    }
    else {
        if(data==NULL)
        {
            qDebug() << "脱离数据库";
            SingleGameN::computerMove();
        }
        else {
            qDebug() << "我开始走了";

            tranferStep(data);
            informationShowInver(_s_s[0],_s_s[2] ,_s_s[3]);
            startPoint=center(_s_s[0]);
            endPoint=center(_s_s[2], _s_s[3]);
            _s[_s_s[0]]._value+= MobilityValues(_s_s[0]);
            moveStone(_s_s[0], _s_s[1], _s_s[2], _s_s[3]);
            qDebug() << "走完了";
            update();
            Board::getWin(_s_s[1]);
        }
    }
}
void SqliteDatabase::dataRead()
{
        for (int i=0;i<10;i++) {
               _s_chess[i] = sql_query.value(i).toString().trimmed();
               //qDebug() << _s_chess[i].toStdWString();
        }


}
void SqliteDatabase::dataRead1()
{

    dataRead();
    QString temp=matchDate();
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

QString SqliteDatabase::dataQuery()
{
    //遍历每个字段匹配数据
    QString select_all_sql = "select * from chess";
    sql_query=QSqlQuery(database);
    sql_query.prepare(select_all_sql);
    QString step1=NULL;

   // bool thennext;
    if( !sql_query.exec())
    {
        qDebug()<<"失败" <<sql_query.lastError();
    }
    else
    {
                     qDebug() <<"进入棋谱";

                     dataRead1();

                     step1=_s_step;
                     if(step1!=NULL)  qDebug() << "路径存在哦";
                     return step1;
      }
    return step1;
}
QString SqliteDatabase::matchDate()
{
    qDebug() << "我在查找哦";
    QString str=NULL,str2=NULL;

    if(_bSide){
        for (int i=0;i<_pace;++i) {
            if(i>=10)
            {return str2;}
            str=str+_s_chess[i];
             qDebug() <<str;
             qDebug() << "我在累加哦";
        }
    }else {
        for (int i=0;i<_pace+1;++i) {
            if(i>=10)
            {return str2;}
            str=str+_s_chess[i];
             qDebug() <<str;
             qDebug() << "我在累加哦";
        }
    }

    QString plte=_plte->document()->toPlainText().trimmed();
//    qDebug()<<plte;
    for (int i=1;i<_turn+1;i++) {
        plte.replace("第"+QString::number(i,10)+"回合","").replace("\n","").remove(QRegExp("\\:")).remove(QRegExp("\\s+"));
//        qDebug()<<plte+"----------------------------------------";
    }
//    qDebug()<<plte;
    str.replace("车","車").replace("马","馬");
    str.replace("１","1").replace("２","2").replace("３","3").replace("４","4").replace("５","5").replace("６","6").replace("７","7").replace("８","8").replace("９","9");

    if(str==plte)
    {


        if(_bSide){
            str2=_s_chess[_pace];
        }else {
            str2=_s_chess[_pace+1];
        }

        qDebug() << "查找到了哦"<<str2;
        _pace+=2;
    }

    qDebug() << "查找结束哦";
    return str2;
}
void SqliteDatabase::tranferStep(QString str)
{
    QString c[4];
    qDebug() <<"开始转换";

    for (int i=0;i<str.length();i++) {
       c[i]=str.mid(i,1);
           qDebug() <<c[i];
    }

    Stone::TYPE d;

    int b1=-1,b2=-1;
    if(_bSide)
    {
        if(c[0]=="车") d= Stone:: CHE  ;
        if(c[0]=="马") d= Stone::MA   ;
        if(c[0]=="炮") d= Stone::PAO  ;
        if(c[0]=="卒") d= Stone::BING ;
        if(c[0]=="将") d= Stone::JIANG;
        if(c[0]=="士") d= Stone::SHI  ;
        if(c[0]=="象") d= Stone::XIANG;

        if(c[1]=="1") b1=0 ;
        if(c[1]=="2") b1=1 ;
        if(c[1]=="3") b1=2 ;
        if(c[1]=="4") b1=3 ;
        if(c[1]=="5") b1=4 ;
        if(c[1]=="6") b1=5 ;
        if(c[1]=="7") b1=6 ;
        if(c[1]=="8") b1=7 ;
        if(c[1]=="9") b1=8 ;

        if(c[3]=="1") b2=1 ;
        if(c[3]=="2") b2=2 ;
        if(c[3]=="3") b2=3 ;
        if(c[3]=="4") b2=4 ;
        if(c[3]=="5") b2=5 ;
        if(c[3]=="6") b2=6 ;
        if(c[3]=="7") b2=7 ;
        if(c[3]=="8") b2=8 ;
        if(c[3]=="9") b2=9 ;

        if(c[1]=="１") b1=0 ;
        if(c[1]=="２") b1=1 ;
        if(c[1]=="３") b1=2 ;
        if(c[1]=="４") b1=3 ;
        if(c[1]=="５") b1=4 ;
        if(c[1]=="６") b1=5 ;
        if(c[1]=="７") b1=6 ;
        if(c[1]=="８") b1=7 ;
        if(c[1]=="９") b1=8 ;

        if(c[3]=="１") b2=1 ;
        if(c[3]=="２") b2=2 ;
        if(c[3]=="３") b2=3 ;
        if(c[3]=="４") b2=4 ;
        if(c[3]=="５") b2=5 ;
        if(c[3]=="６") b2=6 ;
        if(c[3]=="７") b2=7 ;
        if(c[3]=="８") b2=8 ;
        if(c[3]=="９") b2=9 ;



        qDebug() << "字符串拆解完毕";
        qDebug() <<b1<<b2;
        int min=16, max=32;
        if(this->_bRedTurn)
        {
            min = 0;
            max = 16;
            qDebug() << "变";
        }
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
                            _s_s[2]= _s[i]._row+ b2;
                            _s_s[3]= _s[i]._col;
                            qDebug() <<"进";
                        }
                        if(c[2]=="退")
                        {
                            _s_s[2]= _s[i]._row-b2;
                            _s_s[3]= _s[i]._col;
                            qDebug() <<"退";
                        }
                        if(c[2]=="平")
                        {
                            _s_s[2]= _s[i]._row;
                            _s_s[3]= b2-1;
                             qDebug() <<"平";
                        }
                    }else
                     {
                        if(_s[i]._type==Stone::MA)
                        {
                            if(c[2]=="进")
                            {
                                _s_s[3]=abs(_s[i]._col-(b2-1)) ;
                                if(_s_s[3]==1)
                                {
                                    _s_s[2]= _s[i]._row+2;
                                }
                                else {
                                    _s_s[2]= _s[i]._row+1;
                                }
                                _s_s[3]=b2-1;
                               qDebug() <<"进";
                            }
                            if(c[2]=="退")
                            {
                                _s_s[3]= abs(_s[i]._col-(b2-1));
                                if(_s_s[3]==1)
                                {
                                    _s_s[2]= _s[i]._row-2;
                                }
                                else {
                                    _s_s[2]= _s[i]._row-1;
                                }
                                qDebug() <<"退";
                                _s_s[3]=b2-1;
                            }
                        }
                       if(_s[i]._type==Stone::XIANG)
                       {
                           if(c[2]=="进")
                           {

                               _s_s[2]= _s[i]._row+2;

                               _s_s[3]=b2-1;
                              qDebug() <<"进";
                           }
                           if(c[2]=="退")
                           {
                               _s_s[2]= _s[i]._row-2;

                               _s_s[3]=b2-1;
                               qDebug() <<"退";
                           }
                       }
                       if(_s[i]._type==Stone::SHI)
                       {
                           if(c[2]=="进")
                           {

                               _s_s[2]= _s[i]._row+1;

                               _s_s[3]=b2-1;
                              qDebug() <<"进";
                           }
                           if(c[2]=="退")
                           {
                               _s_s[2]= _s[i]._row-1;

                               _s_s[3]=b2-1;
                               qDebug() <<"退";
                           }
                       }
                    }

                    _s_s[1]=getStoneId(_s_s[2],_s_s[3]);


                }
            }
        }
    }
    else {

        if(c[0]=="车") d= Stone:: CHE  ;
        if(c[0]=="马") d= Stone::MA   ;
        if(c[0]=="炮") d= Stone::PAO  ;
        if(c[0]=="兵") d= Stone::BING ;
        if(c[0]=="帅") d= Stone::JIANG;
        if(c[0]=="仕") d= Stone::SHI  ;
        if(c[0]=="相") d= Stone::XIANG;

        if(c[1]=="一") b1=0 ;
        if(c[1]=="二") b1=1 ;
        if(c[1]=="三") b1=2 ;
        if(c[1]=="四") b1=3 ;
        if(c[1]=="五") b1=4 ;
        if(c[1]=="六") b1=5 ;
        if(c[1]=="七") b1=6 ;
        if(c[1]=="八") b1=7 ;
        if(c[1]=="九") b1=8 ;

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
        int min=16, max=32;
        if(this->_bRedTurn)
        {
            min = 0;
            max = 16;
            qDebug() << "变";
        }
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
                            _s_s[2]= _s[i]._row+ b2;
                            _s_s[3]= _s[i]._col;
                            qDebug() <<"进";
                        }
                        if(c[2]=="退")
                        {
                            _s_s[2]= _s[i]._row-b2;
                            _s_s[3]= _s[i]._col;
                            qDebug() <<"退";
                        }
                        if(c[2]=="平")
                        {
                            _s_s[2]= _s[i]._row;
                            _s_s[3]= b2-1;
                             qDebug() <<"平";
                        }
                    }else
                     {
                        if(_s[i]._type==Stone::MA)
                        {
                            if(c[2]=="进")
                            {
                                _s_s[3]=abs(_s[i]._col-(b2-1)) ;
                                if(_s_s[3]==1)
                                {
                                    _s_s[2]= _s[i]._row+2;
                                }
                                else {
                                    _s_s[2]= _s[i]._row+1;
                                }
                                _s_s[3]=b2-1;
                               qDebug() <<"进";
                            }
                            if(c[2]=="退")
                            {
                                _s_s[3]= abs(_s[i]._col-(b2-1));
                                if(_s_s[3]==1)
                                {
                                    _s_s[2]= _s[i]._row-2;
                                }
                                else {
                                    _s_s[2]= _s[i]._row-1;
                                }
                                qDebug() <<"退";
                                _s_s[3]=b2-1;
                            }
                        }
                       if(_s[i]._type==Stone::XIANG)
                       {
                           if(c[2]=="进")
                           {

                               _s_s[2]= _s[i]._row+2;

                               _s_s[3]=b2-1;
                              qDebug() <<"进";
                           }
                           if(c[2]=="退")
                           {
                               _s_s[2]= _s[i]._row-2;

                               _s_s[3]=b2-1;
                               qDebug() <<"退";
                           }
                       }
                       if(_s[i]._type==Stone::SHI)
                       {
                           if(c[2]=="进")
                           {

                               _s_s[2]= _s[i]._row+1;

                               _s_s[3]=b2-1;
                              qDebug() <<"进";
                           }
                           if(c[2]=="退")
                           {
                               _s_s[2]= _s[i]._row-1;

                               _s_s[3]=b2-1;
                               qDebug() <<"退";
                           }
                       }
                    }

                    _s_s[1]=getStoneId(_s_s[2],_s_s[3]);


                }
            }
        }
    }

}
void SqliteDatabase::aiRedGo()
{
    QString select_all_sql = "SELECT * FROM chess ORDER BY random() LIMIT 1";

    qDebug() <<select_all_sql;
    sql_query=QSqlQuery(database);

    sql_query.prepare(select_all_sql);
    sql_query.exec();
    sql_query.first();
    _s_chess[0] = sql_query.value(0).toString().trimmed();

    qDebug()<<"随机开局步子"+_s_chess[0];
//    for (int i=0;i<10;i++) {

//           qDebug() << sql_query.value(i).toString().trimmed();
//    }
//    _s_chess[0]="炮二平五";
//    qDebug()<<"随机开局步子"+_s_chess[0];
    tranferStep(_s_chess[0]);
    informationShowInver(_s_s[0],_s_s[2] ,_s_s[3]);
    startPoint=center(_s_s[0]);
    endPoint=center(_s_s[2], _s_s[3]);
    moveStone(_s_s[0], _s_s[1], _s_s[2], _s_s[3]);
    sql_query.first();
}
void SqliteDatabase::Inverse()
{
    QMessageBox::StandardButton ret=QMessageBox::question(NULL,"WARNING","交换执棋方将强制重开本场比赛！");

    if(ret==QMessageBox::Yes)
    {

        inver=!inver;
        Board:: init(inver);
        _pace=1;
        end=false;
        _back->setStyleSheet("background-color:red;color:white");
        sql_query.first();
        this->_steps.clear();
        this->_plte->clear();
        startPoint=center(-1,-1);
        endPoint=center(-1, -1);
        if(inver){
            _restoreChess->show();
        }else {
            _restoreChess->hide();
        }

        update();

        if(_bRedTurn == inver)
            return;
        aiRedGo();
    }
}
void SqliteDatabase::backStep()
{
    sql_query.first();

    if(_pace!=1)
    {
        _pace-=2;
        if(_pace<0)
        {
            _pace=1;
        }
    }
    startPoint=center(-1,-1);
    endPoint=center(-1, -1);
    Board:: informationDelet();
    Board:: backOne();
    Board:: informationDelet();
    Board:: backOne();
    if (Board:: _steps.size() == 0)
    {
        Board::init(_bSide);
        end=false;
        _back->setStyleSheet("background-color:red;color:white");
        sql_query.first();
        this->_steps.clear();
        this->_plte->clear();
        if(!_bSide){
            aiRedGo();
        }
        update();
    }
}
void SqliteDatabase::draw()
{
    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(NULL,"GameOver","YOU WANT A DRAW \n 点击确认进入下一局比赛");
    if(ret==QMessageBox::Yes)
    {
            _pace=1;
            end=false;
            Board::init(_bSide);
            startPoint=center(-1,-1);
            endPoint=center(-1, -1);
            _back->setStyleSheet("background-color:red;color:white");
            sql_query.first();
            this->_steps.clear();
            this->_plte->clear();
            if(!_bSide){
                aiRedGo();
            }
            update();

    }
}
void SqliteDatabase::getWin(int id)
{
    if (id==-1)
        return;
    if(_s[id]._type==Stone::JIANG&&_s[id]._dead==true)
    {
            QMessageBox::StandardButton ret;
            if(_s[id]._red==_bRedTurn){


                ret=QMessageBox::question(NULL,"GameOver","DEFEATED /n 点击进入下一局比赛");

                if(ret==QMessageBox::Yes)
                {
                    Board:: init(_bSide);


                }
                else {
                    Board:: init(_bSide);

                }
            }
            else {
                ret=QMessageBox::question(NULL,"GameOver","YOU WIN \n 点击进入下一局比赛");

                if(ret==QMessageBox::Yes)
                {
                    Board:: init(_bSide);
                }
                else {
                    Board:: init(_bSide);

                }
            }
            _back->setStyleSheet("background-color:red;color:white");
            this->_steps.clear();
            this->_plte->clear();
            end=false;
            startPoint=center(0,0);
            endPoint=center(0, 0);
            sql_query.first();
            if(!_bSide){
                aiRedGo();
            }
           update();
    }
}
