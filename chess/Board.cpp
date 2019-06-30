#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include<QMessageBox>
#include<QLabel>
#include<math.h>
#include<stdlib.h>
#include <QPlainTextEdit>
#include <QFile>
#include<QFileDialog>
#include<QDateTime>
#include <QTextCodec>
#include<QApplication>
#include <QPropertyAnimation>
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) : QFrame(parent)
{
    this->_r = 40;
    setMinimumSize(_r*20, _r*20+40);
    setMaximumSize(_r*30, _r*20+40);

    inforText();
    backButton();
    drawButton();
    saveChessButton();
    restoreChessButton();
    QPixmap pixmap = QPixmap("2.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->grabKeyboard();
    init(true);
    this->setMouseTracking(true);
}
void Board::drawButton()
{
    _draw=new QPushButton("求和");
    _draw->setFixedSize(_r*4,_r*2);
    _draw->setStyleSheet("background-color:orange;color:blue");
    _draw->setWindowOpacity(0.5);
    _draw->move(_r*24+30,_r*15);
    _draw->setFont(QFont("Times", 20, QFont::Bold));

    _draw->show();
    _draw->setParent(this);
    connect(_draw,&QPushButton::clicked,this,&Board::draw);
    startPoint=center(0,0);
    endPoint=center(0, 0);

}
void Board::inforText()
{

    _plte->setReadOnly(true);
    _plte->clear();

    _plte->setWindowOpacity(0.5);
    _plte->setParent(this);
     _plte ->move(_r*21+10,_r*1);
     _plte->setFixedSize(_r*7,_r*12);
     _plte->setFont(QFont("Times", 16, QFont::Bold));
     _plte->show();

}
void Board::backButton()
{
    _back=new QPushButton("悔棋");
    _back->setFixedSize(_r*4,_r*2);
    _back->setStyleSheet("background-color:red;color:white");

    _back->move(_r*20+20,_r*15);
    _back->setFont(QFont("Times", 20, QFont::Bold));

    _back->show();
    _back->setParent(this);
    connect(_back,&QPushButton::clicked,this,&Board::backStep);

}
void Board::saveChessButton()
{
    _saveChess=new QPushButton("保存战局");
    _saveChess->setFixedSize(_r*4,_r*2);
    _saveChess->setStyleSheet("background-color:purple;color:white");

    _saveChess->move(_r*20+20,_r*17+5);
    _saveChess->setFont(QFont("Times", 20, QFont::Bold));

    _saveChess->show();
    _saveChess->setParent(this);
    connect(_saveChess,&QPushButton::clicked,this,&Board::saveChess);
}
void Board::saveChess()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date ="Endgame/"+current_date_time.toString("yyyy.MM.dd.hh.mm.ddd")+".txt";
//    QString sssss="10";
//    current_date="Endgame/"+sssss+".txt";
    //实例QFile
    QFile file(current_date);
    //定义文件内容字符串
    QString ssssss=NULL;
    if(_bSide)
    {
        ssssss.append(QString::number(1,10));
    }
    else {
        ssssss.append(QString::number(0,10));
    }

    ssssss.append("\n");
    if(_bRedTurn)
    {
        ssssss.append(QString::number(1,10));
    }
    else {
        ssssss.append(QString::number(0,10));
    }

    ssssss.append("\n");
//    for (int i=0;i<32;i++) {
//        ssssss.append(QString::number(i,10));
//        ssssss.append(" ");
//        ssssss.append(QString::number(_s[i]._row,10));
//        ssssss.append(" ");
//        ssssss.append(QString::number(_s[i]._col,10));
//        ssssss.append(" ");
//        if(_s[i]._dead==true)
//        {
//            ssssss.append(QString::number(1,10));
//        }
//        else {
//            ssssss.append(QString::number(0,10));
//        }
//        ssssss.append("\n");
//    }
    ssssss.append(_plte->document()->toPlainText().trimmed().replace("\n       "," "));




    QString content= ssssss;


    //判断文件是否存在
    if(file.exists())
    {
        QMessageBox::warning(this,"创建文件","文件已经存在！");
    }else
    {
        //存在打开，不存在创建
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        //写入内容,这里需要转码，否则报错。
        QByteArray str = content.toUtf8();
        //写入QByteArray格式字符串
        file.write(str);
        //提示成功
        QMessageBox::warning(this,"创建文件","文件创建成功！");
    }
    //关闭文件
    file.close();
}
Board::~Board()
{

}
void Board::restoreChessButton()
{
    _restoreChess=new QPushButton("读取战局");
    _restoreChess->setFixedSize(_r*4,_r*2);
    _restoreChess->setStyleSheet("background-color:aqua;color:blue");

    _restoreChess->move(_r*20+20,_r*13-5);
    _restoreChess->setFont(QFont("Times", 20, QFont::Bold));

   _restoreChess->show();
   _restoreChess->setParent(this);
    connect(_restoreChess,&QPushButton::clicked,this,&Board::restoreChess);
}
void Board::restoreChess()
{
    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(NULL,"提示","点击确认可以选择保存的战局");
    if(ret==QMessageBox::Yes){

        Board::init(true);
        _plte->clear();
        QString fileName;
        fileName=QFileDialog::getOpenFileName(this,tr("文件"),"Endgame",tr("text(*.txt)"));////括号里的参数分别是：指定父类、标题、默认打开后显示的目录、右下角的文件过滤器。
        if(!fileName.isNull())
        {
            QFile file(fileName);
            if(!file.open(QFile::ReadOnly|QFile::Text))
            {
                QMessageBox::warning(this,tr("Error"),tr("读取失败请重试").arg(file.errorString()));
                return;

            }
            QTextStream in(&file);
            int i=0;
            _plte->clear();
            QString strLine;
            in.setCodec("UTF-8");
            while (!in.atEnd())
            {
                strLine = in.readLine();
                strLine.remove("\r\n");

                QStringList sections = strLine.split(QRegExp( "[|\\s+]"));//“ \\s+” 匹配空格的
                if(i==0)
                {
//                    if(sections.at(0)=="1")
//                    {
//                        _bSide=true;
//                    }
//                    else {
//                        _bSide=false;
//                    }
                }
                if (i==1)
                {
                    if(sections.at(0)=="1")
                    {
                        _bRedTurn=true;
                    }
                    else {
                        _bRedTurn=false;
                    }
                }
    //            if(i>1&&i<34)
    //            {
    //                _s[i-2]._row=sections.at(1).toInt();
    //                _s[i-2]._col=sections.at(2).toInt();
    //                if(sections.at(3)=="1")
    //                {
    //                    _s[i-2]._dead=true;
    //                }
    //                else {
    //                    _s[i-2]._dead=false;
    //                }
    //            }
                if(i>1)
                {
                    QStringList sections2 = strLine.split(QRegExp( "[|\\:]"));
                    QString str =sections2.at(0);
                    _turn=str.remove("第").remove("回合").toInt()+1;

                    if(sections.length()==2)
                    {
                        _plte->appendPlainText(sections.at(0)+"\n       "+sections.at(1));//乱码
                        restoreGo(1,sections.at(0));
                        restoreGo(2,sections.at(1));
                    }
                    else {
                        _plte->appendPlainText(sections.at(0));
                        restoreGo(1,sections.at(0));
                    }

    //                qDebug()<<_plte->document()->toPlainText();
    //                QStringList sections2 = strLine.split(QRegExp( "[|\\:]"));
    //                QString str =sections2.at(0);
    //                _turn=str.remove("第").remove("回合").toInt()+1;

                }
                i++;

            }
        }
        _back->setEnabled(false);
        _PushKey=true;
        _stepsForRestore=_steps;
        _stepsForRestoreCount= _stepsForRestore.size();
    //    _steps.clear();
        _bRedTurn=!_bRedTurn;
        turnColor();
        _bRedTurn=!_bRedTurn;
        update();
    }


}

void Board::restoreGo(int red, QString str)
{

    for (int i=1;i<_turn+1;i++) {
        str.replace("第"+QString::number(i,10)+"回合","").replace("\n","").remove(QRegExp("\\:")).remove(QRegExp("\\s+"));
    }
    str.replace("車","车").replace("馬","马");
    str.replace("１","1").replace("２","2").replace("３","3").replace("４","4").replace("５","5").replace("６","6").replace("７","7").replace("８","8").replace("９","9");

    int s_s[4];
    if(red==1)
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
//                qDebug() << "纠错2";
                //qDebug() << "尝试找到棋子";
                if(_s[i]._row == row &&_s[i]._col == b1 && _s[i]._type == d)
                {
                    qDebug() << "找到棋子";

                    s_s[0]=i;

                    qDebug() << "记录初始点";
                    if(_s[i]._type==Stone::CHE||_s[i]._type==Stone::PAO||_s[i]._type==Stone::BING||_s[i]._type==Stone::JIANG)
                    {
                        if(c[2]=="进")
                        {
                            s_s[2]= _s[i]._row-b2;
                            s_s[3]= _s[i]._col;
                            qDebug() <<"进";
                        }
                        if(c[2]=="退")
                        {
                            s_s[2]= _s[i]._row+b2;
                            s_s[3]= _s[i]._col;
                            qDebug() <<"退";
                        }
                        if(c[2]=="平")
                        {
                            s_s[2]= _s[i]._row;
                            s_s[3]=abs(8-(b2-1)) ;
                             qDebug() <<"平";
                        }
                    }else
                     {
                        if(_s[i]._type==Stone::MA)
                        {
                            if(c[2]=="进")
                            {
                                s_s[3]=abs(_s[i]._col-(8-(b2-1))) ;
                                if(s_s[3]==1)
                                {
                                    s_s[2]= _s[i]._row-2;
                                }
                                else {
                                    s_s[2]= _s[i]._row-1;
                                }
                                s_s[3]=abs(8-(b2-1));
                               qDebug() <<"进";
                            }
                            if(c[2]=="退")
                            {
                                s_s[3]= abs(_s[i]._col-(8-(b2-1)));
                                if(s_s[3]==1)
                                {
                                    s_s[2]= _s[i]._row+2;
                                }
                                else {
                                    s_s[2]= _s[i]._row+1;
                                }
                                qDebug() <<"退";
                                s_s[3]=abs(8-(b2-1));
                            }
                        }
                       if(_s[i]._type==Stone::XIANG)
                       {
                           if(c[2]=="进")
                           {

                               s_s[2]= _s[i]._row-2;

                               s_s[3]=abs(8-(b2-1));
                              qDebug() <<"进";
                           }
                           if(c[2]=="退")
                           {
                               s_s[2]= _s[i]._row+2;

                               s_s[3]=abs(8-(b2-1));
                               qDebug() <<"退";
                           }
                       }
                       if(_s[i]._type==Stone::SHI)
                       {
                           if(c[2]=="进")
                           {

                               s_s[2]= _s[i]._row-1;

                               s_s[3]=abs(8-(b2-1));
                              qDebug() <<"进";
                           }
                           if(c[2]=="退")
                           {
                               s_s[2]= _s[i]._row+1;

                               s_s[3]=abs(8-(b2-1));
                               qDebug() <<"退";
                           }
                       }
                    }

                    s_s[1]=getStoneId(s_s[2],s_s[3]);


                }
            }
        }
    }
    else {

        QString c[4];
        qDebug() <<"开始转换";

        for (int i=0;i<str.length();i++) {
           c[i]=str.mid(i,1);
               qDebug() <<c[i];
        }

        Stone::TYPE d;

        int b1=-1,b2=-1;

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

                        s_s[0]=i;

                        qDebug() << "记录初始点";
                        if(_s[i]._type==Stone::CHE||_s[i]._type==Stone::PAO||_s[i]._type==Stone::BING||_s[i]._type==Stone::JIANG)
                        {
                            if(c[2]=="进")
                            {
                                s_s[2]= _s[i]._row+ b2;
                                s_s[3]= _s[i]._col;
                                qDebug() <<"进";
                            }
                            if(c[2]=="退")
                            {
                                s_s[2]= _s[i]._row-b2;
                                s_s[3]= _s[i]._col;
                                qDebug() <<"退";
                            }
                            if(c[2]=="平")
                            {
                                s_s[2]= _s[i]._row;
                                s_s[3]= b2-1;
                                 qDebug() <<"平";
                            }
                        }else
                         {
                            if(_s[i]._type==Stone::MA)
                            {
                                if(c[2]=="进")
                                {
                                    s_s[3]=abs(_s[i]._col-(b2-1)) ;
                                    if(s_s[3]==1)
                                    {
                                        s_s[2]= _s[i]._row+2;
                                    }
                                    else {
                                        s_s[2]= _s[i]._row+1;
                                    }
                                    s_s[3]=b2-1;
                                   qDebug() <<"进";
                                }
                                if(c[2]=="退")
                                {
                                    s_s[3]= abs(_s[i]._col-(b2-1));
                                    if(s_s[3]==1)
                                    {
                                        s_s[2]= _s[i]._row-2;
                                    }
                                    else {
                                        s_s[2]= _s[i]._row-1;
                                    }
                                    qDebug() <<"退";
                                    s_s[3]=b2-1;
                                }
                            }
                           if(_s[i]._type==Stone::XIANG)
                           {
                               if(c[2]=="进")
                               {

                                   s_s[2]= _s[i]._row+2;

                                   s_s[3]=b2-1;
                                  qDebug() <<"进";
                               }
                               if(c[2]=="退")
                               {
                                   s_s[2]= _s[i]._row-2;

                                   s_s[3]=b2-1;
                                   qDebug() <<"退";
                               }
                           }
                           if(_s[i]._type==Stone::SHI)
                           {
                               if(c[2]=="进")
                               {

                                   s_s[2]= _s[i]._row+1;

                                   s_s[3]=b2-1;
                                  qDebug() <<"进";
                               }
                               if(c[2]=="退")
                               {
                                   s_s[2]= _s[i]._row-1;

                                   s_s[3]=b2-1;
                                   qDebug() <<"退";
                               }
                           }
                        }

                        s_s[1]=getStoneId(s_s[2],s_s[3]);


                    }
                }
            }
        }
//    startPoint=center(s_s[0]);
//    endPoint=center(s_s[2], s_s[3]);
    _s[s_s[0]]._value+= MobilityValues(s_s[0]);
    moveStone(s_s[0], s_s[1], s_s[2], s_s[3]);

    qDebug() << "走完了";
    update();
}
void Board::init(bool bRedSide)
{
    for(int i=0; i<32; ++i)
    {
        _s[i].init(i);
    }
    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            _s[i].rotate();
        }
    }

    _selectid = -1;
    _bRedTurn = true;
    _bSide = bRedSide;
    startPoint=center(-1,-1);
    endPoint=center(-1, -1);
    update();
}

void Board::paintEvent(QPaintEvent *)
{
    int r = height()/20;
    _r = r;
    _off = QPoint(r+1, r+1);

    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.save();
    drawPlate(p);
    p.restore();

    p.save();
    drawPlace(p);
    p.restore();

    p.save();
    drawInitPosition(p);
    p.restore();

    p.save();
    drawStone(p);
    p.restore();

//    p.save();
//    if(row11>0){
//        startPoint=center(row11,col11);
//    }

     drawArrow(p);


//    p.restore();
}

void Board::drawStone(QPainter &p)
{
    for(int i=0; i<32; i++)
    {
        drawStone(p, i);
    }
}

bool Board::isDead(int id)
{
    if(id == -1)return true;
    return _s[id]._dead;

}

void Board::drawStone(QPainter &p, int id)
{
    if(isDead(id)) return;
    int d=2*_r;
    QColor color;
    if(red(id)) color = Qt::red;
    else color = Qt::black;
    p.setFont(QFont("Times", d/2, QFont::Bold));//字体设置
    p.setPen(QPen(QBrush(color), 2));

    if(id == _selectid) p.setBrush(Qt::green);
    else p.setBrush(Qt::yellow);

    p.drawEllipse(cell(id));
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));

    p.setRenderHint(QPainter::Antialiasing);//抗锯齿


}

QString Board::name(int id)
{
    return _s[id].name();
}

bool Board::red(int id)
{
    return _s[id]._red;
}

void Board::drawInitPosition(QPainter &p, int row, int col)
{
    QPoint pt = center(row, col);
    QPoint off = QPoint(_r/6, _r/6);
    int len = _r/3;

    QPoint ptStart;
    QPoint ptEnd;

    if(col != 0)
    {
        /* 左上角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);

        /* 左下角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);
    }

    if(col != 8)
    {
        /* 右下角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);

        /* 右上角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);
    }
}

void Board::drawInitPosition(QPainter &p)
{
    drawInitPosition(p, 3, 0);
    drawInitPosition(p, 3, 2);
    drawInitPosition(p, 3, 4);
    drawInitPosition(p, 3, 6);
    drawInitPosition(p, 3, 8);

    drawInitPosition(p, 6, 0);
    drawInitPosition(p, 6, 2);
    drawInitPosition(p, 6, 4);
    drawInitPosition(p, 6, 6);
    drawInitPosition(p, 6, 8);

    drawInitPosition(p, 2, 1);
    drawInitPosition(p, 2, 7);

    drawInitPosition(p, 7, 1);
    drawInitPosition(p, 7, 7);
}

void Board::drawPlace(QPainter &p)
{
    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));

    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}

void Board::drawPlate(QPainter &p)
{


    for(int i=0; i<10; ++i)//行
    {
        if(i==0 || i==9)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        p.drawLine(center(i, 0), center(i, 8));
    }

    for(int i=0; i<9; ++i)//列
    {
        if(i==0 || i==8)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }


    int d=2*_r;
    QRect rect(d/2, 5*d-_r, 8*d, d);//QRect ( int left, int top, int width, int height )
    p.setFont(QFont("Times", d/2, QFont::Bold));//字体设置
    //painter.drawRect(rect);//显示矩形边界
    // 居中绘制文本
    QFontMetrics metrics = p.fontMetrics();
    int stringHeight = metrics.ascent() + metrics.descent(); // 不算 line gap
    int stringWidth = metrics.width("楚 河 汉 界"); // 字符串的宽度
    int x = rect.x() + (rect.width() - stringWidth) / 2;
    int y = rect.y() + (rect.height() - stringHeight) / 2 + metrics.ascent();
    p.drawText(x, y, "楚 河 汉 界");

}

QPoint Board::center(int row, int col)
{
    QPoint pt(_r*col*2, _r*row*2);
    return pt + _off;
}

QPoint Board::center(int id)
{
    return center(_s[id]._row, _s[id]._col);
}

QPoint Board::topLeft(int row, int col)
{
    return center(row, col) - QPoint(_r, _r);
}

QPoint Board::topLeft(int id)
{
    return center(id) - QPoint(_r, _r);
}

QRect Board::cell(int row, int col)
{
    return QRect(topLeft(row, col), QSize(_r*2-1, _r*2-1));
}

QRect Board::cell(int id)
{
    return QRect(topLeft(id), QSize(_r*2-1, _r*2-1));
}

bool Board::getClickRowCol(QPoint pt, int &row, int &col)
{
    for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            QPoint distance = center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < _r* _r)
                return true;
        }
    }
    return false;
}

bool Board::canSelect(int id)
{
    return _bRedTurn == _s[id]._red;
}

void Board::trySelectStone(int id)
{
    if(id == -1)
        return;

    if(!canSelect(id)) return;

    _selectid = id;
    update();
}

bool Board::sameColor(int id1, int id2)
{
    if(id1 == -1 || id2 == -1) return false;

    return red(id1) == red(id2);
}

void Board::tryMoveStone(int killid, int row, int col)
{
    if(killid != -1 && sameColor(killid, _selectid))
    {
        trySelectStone(killid);
        return;
    }

    bool ret = canMove(_selectid, killid, row, col);
    if(ret)
    {
        if(!_PushKey)
        {
            if(_bSide)
            {
              informationShow(_selectid,  row,  col);
            }
            else {
              informationShowInver(_selectid,  row,  col);
            }
        }

//        informationShow(_selectid,  row,  col);
        startPoint=center(_selectid);
        endPoint=center(row, col);
        moveStone(_selectid, killid, row, col);

        _selectid = -1;

        update();

    }
}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getStoneId(row1, col) != -1) ++ret;
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getStoneId(row, col1) != -1) ++ret;
        }
    }

    return ret;
}

int Board::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);
}

bool Board::canMoveChe(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    if(ret == 0)
        return true;
    return false;
}

bool Board::canMoveMa(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 12 && r != 21)
        return false;

    if(r == 12)
    {
        if(getStoneId(row1, (col+col1)/2) != -1)
            return false;
    }
    else
    {
        if(getStoneId((row+row1)/2, col1) != -1)
            return false;
    }

    return true;
}

bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row, col, row1, col1);
    if(killid != -1)
    {
        if(ret == 1) return true;
    }
    else
    {
        if(ret == 0) return true;
    }
    return false;
}

bool Board::canMoveBing(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    if(isBottomSide(moveid))
    {
        if(row > row1) return false;
        if(row1 >= 5 && row == row1) return false;
    }
    else
    {
        if(row < row1) return false;
        if(row1 <= 4 && row == row1) return false;
    }

    return true;
}

bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
    if(killid != -1 && _s[killid]._type == Stone::JIANG)
        return canMoveChe(moveid, killid, row, col);

    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    if(col < 3 || col > 5) return false;
    if(isBottomSide(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

bool Board::canMoveShi(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 11) return false;

    if(col < 3 || col > 5) return false;
    if(isBottomSide(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

bool Board::canMoveXiang(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 22) return false;

    int rEye = (row+row1)/2;
    int cEye = (col+col1)/2;
    if(getStoneId(rEye, cEye) != -1)
        return false;

    if(isBottomSide(moveid))
    {
        if(row < 4) return false;
    }
    else
    {
        if(row > 5) return false;
    }
    return true;
}

bool Board::canMove(int moveid, int killid, int row, int col)
{
    if(sameColor(moveid, killid)) return false;

    switch (_s[moveid]._type)
    {
    case Stone::CHE:
        return canMoveChe(moveid, killid, row, col);

    case Stone::MA:
        return canMoveMa(moveid, killid, row, col);

    case Stone::PAO:
        return canMovePao(moveid, killid, row, col);

    case Stone::BING:
        return canMoveBing(moveid, killid, row, col);

    case Stone::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case Stone::SHI:
        return canMoveShi(moveid, killid, row, col);

    case Stone::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    }
    return false;
}

void Board::reliveStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = false;
}

void Board::killStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = true;

}

bool Board::isBottomSide(int id)
{
    return _bSide == _s[id]._red;
}

void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._row = row;
    _s[moveid]._col = col;
    turnColor();
    _bRedTurn = !_bRedTurn;
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}

void Board::moveStone(int moveid, int killid, int row, int col)
{
    if(_PushKey)
    {
        init(true);
        _plte->clear();
        _steps.clear();
        for (int i=0;i<_stepsForRestoreCount;i++) {
            _steps.append(_stepsForRestore.at(i));
        }

        _turn=1;
        for (int i=0;i<_steps.size();i++)
        {
            killStone(_steps.at(i)->_killid);
            informationShow(_steps.at(i)->_moveid,_steps.at(i)->_rowTo,_steps.at(i)->_colTo);
            moveStone(_steps.at(i)->_moveid, _steps.at(i)->_rowTo, _steps.at(i)->_colTo);
            qDebug()<<"我在前进";
        }
        _back->setEnabled(true);

    }
    if(_PushKey)
    {
        if(_bSide)
        {
          informationShow(moveid,  row,  col);
        }
        else {
          informationShowInver(moveid,  row,  col);
        }
        startPoint=center(moveid);
        endPoint=center(row, col);
        _PushKey=false;
    }
    saveStep(moveid, killid, row, col, _steps);
    killStone(killid);
    moveStone(moveid, row, col);


}

void Board::click(int id, int row, int col)
{
    if(this->_selectid == -1)
    {

        trySelectStone(id);
    }
    else
    {
//        row11=_s[id]._row;
//        col11=_s[id]._col;
//        qDebug()<<"_s[id]._row"+QString::number(_s[_selectid]._row,10);
//        qDebug()<<"_s[id]._row"+QString::number(_s[_selectid]._col,10);

//enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
//        int MobilityValues[7] = { 6,  12,  6,  15, 0,  1,   1 };


        if(_s[_selectid]._value<100)
            _s[_selectid]._value = MobilityValues(_selectid);

        tryMoveStone(id, row, col);
        checkmate();


    }

    getWin(id);
}

void Board::click(QPoint pt)
{
    int row, col;
    bool bClicked = getClickRowCol(pt, row, col);
    if(!bClicked) return;

    int id = getStoneId(row, col);
    click(id, row, col);
}

int Board::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && !isDead(i))
            return i;
    }
    return -1;
}

void Board::back(Step *step)
{
    //只要将moveId的行列修改就可以了
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

void Board::backOne()
{
//恢复棋子的所有状态
//刚开始棋局，那么悔棋无效
    if(this->_steps.size() == 0) return;

    Step* step = this->_steps.last();
    _steps.removeLast();
    back(step);

    update();
    delete step;
}

void Board::back()
{
    backOne();
}

void Board::slotBack()
{
    back();
}
void Board::backStep()
{
    informationDelet();

    backOne();
    startPoint=center(-1,-1);
    endPoint=center(-1, -1);
}
//    if(this->_steps.size() == 0) return;
//    Step* step = this->_steps.last();

//    //上一步操作将棋子移动到空白位置
//    if(step->_killid==-1)
//    {
//        //只要将moveId的行列修改就可以了
//        _s[step->_moveid]._row=step->_rowFrom;
//        _s[step->_moveid]._col=step->_colFrom;
//         turnColor();
//        _bRedTurn=!_bRedTurn;
//        update();
//    }
//    else
//    {
//        //需要将killId棋子状态恢复，将moveId棋子状态恢复
//        _s[step->_moveid]._row=step->_rowFrom;
//        _s[step->_moveid]._col=step->_colFrom;
//        _s[step->_killid]._dead=false;
//        _s[step->_killid]._row=step->_rowTo;
//        _s[step->_killid]._col=step->_colTo;
//         turnColor();
//         _bRedTurn=!_bRedTurn;
//        update();
//    }
//    _steps.removeLast();
//    delete step;
//    update();

void Board::turnColor()
{
    if(_bRedTurn)
    {
         _back->setStyleSheet("background-color:black;color:white");

    }else {

        _back->setStyleSheet("background-color:red;color:white");
    }
}
void Board::getWin(int id)
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
           update();
    }
}
void Board::informationShow(int moveid, int row ,int col)
{
    //显示
    QString str;

    QString str2;//动作
    QString str3;//确认走的棋子是不是能直走并保存距离
    QString str4=NULL;
    for (int i=0;i<32;i++) {
        if(_s[i]._dead)
            continue;
        if(i==moveid)
        {}
        else {
            if(_s[i]._col==_s[moveid]._col&&_s[i]._red==_s[moveid]._red&&_s[i]._type==_s[moveid]._type)
            {
                    if(!_s[moveid]._red){
                        if (_s[moveid]._row<_s[i]._row)
                        {str4="后";}
                        else{str4="前";}
                    }else {
                        if (_s[moveid]._row>_s[i]._row)
                        {str4="后";}
                        else{str4="前";}
                    }

            }
        }

    }

    if(!_s[moveid]._red){
        if (_s[moveid]._row<row)
        {str2="进";}
        else{str2="退";}
    }else {
        if (_s[moveid]._row<row)
        {str2="退";}
        else{str2="进";}
    }
    if (_s[moveid]._row==row)
        str2="平";
    bool isred=(_s[moveid]._red==_bSide);
    if(_s[moveid]._type==Stone::CHE||_s[moveid]._type==Stone::PAO||_s[moveid]._type==Stone::BING||_s[moveid]._type==Stone::JIANG)
    {
        if(str2=="进"||str2=="退")
            {
            if(_s[moveid]._red){
                 str3=infor( isred,9-abs( _s[moveid]._row-row));
            }

              else {
                  str3=infor( isred,abs( _s[moveid]._row-row)-1);
              }

        }
        else {
            str3=infor(isred,col);
        }
    }else {
            str3=infor(isred,col);
    }
    QString str5;
    if(_s[moveid]._red)
    {
        str5="第"+QString::number(_turn,10)+"回合:";
        _turn++;
    }else {
         str5="       ";
    }

    if(str4=="前"||str4=="后")
    {
        str=str5+ str4+ name(moveid)+str2+str3;
    }
    else {
        str=str5+name(moveid)+infor(isred,_s[moveid]._col)+str2+str3;
    }

    _plte->appendPlainText(str);
    //qDebug()<<_plte->document()->toPlainText().trimmed().replace("\n","");
    //qDebug()<<_plte->document()->toPlainText().trimmed().replace("\n","").length();
}
QString Board::infor(bool isred,int col)
{
    QString str1;
    if(isred)
    {
        switch (col)
        {
            case 0 : str1="九";break;
            case 1 : str1="八";break;
            case 2 : str1="七";break;
            case 3 : str1="六";break;
            case 4 : str1="五";break;
            case 5 : str1="四";break;
            case 6 : str1="三";break;
            case 7 : str1="二";break;
            case 8 : str1="一";break;
        }

    }
    else
    {
        switch (col)
        {
            //case -1:str1="0";break;//这一个是为了显示而专门设计
            case 0 : str1="1";break;
            case 1 : str1="2";break;
            case 2 : str1="3";break;
            case 3 : str1="4";break;
            case 4 : str1="5";break;
            case 5 : str1="6";break;
            case 6 : str1="7";break;
            case 7 : str1="8";break;
            case 8 : str1="9";break;
        }

    }
    return str1;

}
void Board::informationShowInver(int moveid, int row, int col)
{
    //显示
    QString str;

    QString str2;//动作
    QString str3;//确认走的棋子是不是能直走并保存距离
    QString str4=NULL;
    for (int i=0;i<32;i++) {
        if(_s[i]._dead)
            continue;
        if(i==moveid)
        {

        }else {

            if(_s[i]._col==_s[moveid]._col&&_s[i]._red==_s[moveid]._red)
            {
                if( _s[i]._type==_s[moveid]._type)
                {
                    if(!_s[moveid]._red){
                        if (_s[moveid]._row>_s[i]._row)
                        {str4="后";}
                        else{str4="前";}
                    }else {
                        if (_s[moveid]._row<_s[i]._row)
                        {str4="后";}
                        else{str4="前";}
                    }

                }
            }
        }


    }
    if(moveid<16){
        if (_s[moveid]._row<row)
        {str2="进";}
        else{str2="退";}
    }else {
        if (_s[moveid]._row<row)
        {str2="退";}
        else{str2="进";}
    }
    if (_s[moveid]._row==row)
        str2="平";

    if(_s[moveid]._type==Stone::CHE||_s[moveid]._type==Stone::PAO||_s[moveid]._type==Stone::BING||_s[moveid]._type==Stone::JIANG)
    {
        if(str2=="进"||str2=="退")
        {
            if(moveid<16){
                 str3=inforInver( moveid,abs( _s[moveid]._row-row)-1);

            }
            else {
                  str3=inforInver( moveid,9-abs( _s[moveid]._row-row));

             }

        }
        else {
            str3=inforInver(moveid,col);
        }
    }else {
            str3=inforInver(moveid,col);
    }
    QString str5;
    if(_s[moveid]._red)
    {
        str5="第"+QString::number(_turn,10)+"回合:";
        _turn++;
    }else {
        str5="       ";
    }

     if(str4=="前"||str4=="后")
    {
        str=str5+str4+ name(moveid)+str2+str3;
    }
    else {
        str=str5+name(moveid)+inforInver(moveid,_s[moveid]._col)+str2+str3;//车六平四
    }


    _plte->appendPlainText(str);
//    for (int i=0;i<32;i++) {
//        if(_s[i]._red==false)
//            qDebug() <<i;
//    }
}
QString Board::inforInver(int s,int col)
{
    QString str1;
    if(s<16)
    {
        switch (col)
        {
            case 8 : str1="九";break;
            case 7 : str1="八";break;
            case 6 : str1="七";break;
            case 5 : str1="六";break;
            case 4 : str1="五";break;
            case 3 : str1="四";break;
            case 2 : str1="三";break;
            case 1 : str1="二";break;
            case 0 : str1="一";break;
        }

    }
    else
    {
        switch (col)
        {
            //case -1:str1="0";break;//这一个是为了显示而专门设计
            case 8 : str1="1";break;
            case 7 : str1="2";break;
            case 6 : str1="3";break;
            case 5 : str1="4";break;
            case 4 : str1="5";break;
            case 3 : str1="6";break;
            case 2 : str1="7";break;
            case 1 : str1="8";break;
            case 0 : str1="9";break;
        }

    }



    return str1;

}
void Board::informationDelet()
{

    if(_plte->document()->toPlainText().size()==0)
        return;


    QString ssssss=_plte->document()->toPlainText().trimmed().replace("\n       ","");

    QStringList sections = ssssss.split(QRegExp( "[|\\n]"));
    for (int i=0;i<sections.size();i++){
        QStringList sections2 = sections.at(i).split(QRegExp( "[|\\:]"));
        QString str =sections2.at(0);
        _turn=str.remove("第").remove("回合").toInt();
        if(_bRedTurn)
        {
            _turn=str.remove("第").remove("回合").toInt()+1;
        }
    }
   _plte->document()->undo();
}
void Board::draw()
{
    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(NULL,"GameOver","YOU WANT A DRAW \n 点击确认进入下一局比赛");
    if(ret==QMessageBox::Yes)
    {

            Board::init(_bSide);
            startPoint=center(-1,-1);
            endPoint=center(-1, -1);
            _turn=1;
            _back->setStyleSheet("background-color:red;color:white");
            this->_steps.clear();
            this->_plte->clear();
            update();

    }


}
void Board::checkmate()
{
    int  a=0,b=0,c=0,d=0,e=0;
    QVector<Step *> stepscheck;

    int min=16, max=32;
    if(!_bRedTurn)
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
                       a+=1;
//                    saveStep(i, killid, row, col, stepscheck);
//                     qDebug()<<"我进行a了哦";
                    if(killid>0)
                    {
                        if(_s[killid]._type==Stone::JIANG)
                        {
    //                       qDebug()<<"我进行b了哦";
                            if(!_bRedTurn)
                            {
                                if(killid>16)
                                {
                                    b+=1;
                                }

                            }
                            if(_bRedTurn)
                            {
                                if(killid<16)
                                {
                                    b+=1;
                                }
                            }

                       }
                    }

                }
            }
        }
    }
    qDebug()<<"我进行判断了哦";


    if(b>0)
    {
            if(min==0)
            {
                min=16;
                max=32;
            }else {
                min=0;
                max=16;
            }
            qDebug()<<"b>0";

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
                             saveStep(i, killid, row, col, stepscheck);
                         }


                    }
                }
            }
            if(min==0)
            {
                min=16;
                max=32;
            }else {
                min=0;
                max=16;
            }

            e=stepscheck.count();
            while(stepscheck.count())
            {

                Step* step = stepscheck.back();
                stepscheck.removeLast();

                killStone(step->_killid);
                moveStone(step->_moveid, step->_rowTo, step->_colTo);//反转
//                qDebug()<<"我进行假走";
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
//                                qDebug()<<"我进行尝试将军呢";

                                d+=1;
                                if(killid>0)
                                {
                                    if(_s[killid]._type==Stone::JIANG)
                                    {

                                        if(_bRedTurn)
                                        {
                                            if(killid>16)
                                            {
                                                c+=1;
                                            }

                                        }
                                        if(!_bRedTurn)
                                        {
                                            if(killid<16)
                                            {
                                               c+=1;
                                            }
                                        }


                                   }
                                }

                            }
                        }
                    }
                }
//               qDebug()<<"我尝试回退假走";
                reliveStone(step->_killid);
                moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
                delete step;
            }

        if(e==c)
        {

            QMessageBox::information(NULL,"           ","绝杀");
        }else {

//            checkanimation(1);

            QMessageBox::information(NULL,"           ","将军");
        }


    }
//    if(b>0)    qDebug()<<"b>0";
//    qDebug()<<QString::number(a,10);
//    qDebug()<<QString::number(b,10);
//    qDebug()<<QString::number(c,10);
//    qDebug()<<QString::number(d,10);
//    qDebug()<<QString::number(e,10);
}
//void Board::checkanimation(int code)
//{

//}
void Board::drawArrow(QPainter &p)
{

    int row11,col11;
    int row22,col22;
    getClickRowCol(startPoint, row11, col11);
    getClickRowCol(endPoint, row22, col22);

    int id11 = getStoneId(row11, col11);
    int id22= getStoneId(row22, col22);
    if(id11>0&&id22>0)
    {
        if(_s[id11]._red==_s[id22]._red)
        {return;}
    }
//    if(!canMove(id11,id22,row22,col22))
//    {
//        return;
//    }
    double par = 16.0;//箭头部分三角形的腰长
    double slopy = atan2((endPoint.y() - startPoint.y()), (endPoint.x() - startPoint.x()));
    double cosy = cos(slopy);
    double siny = sin(slopy);
    QPoint point1 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)), endPoint.y() + int(-par*siny + (par / 2.0*cosy)));
    QPoint point2 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny)), endPoint.y() - int(par / 2.0*cosy + par*siny));
    QPoint points[3] = { endPoint, point1, point2 };

    p.setRenderHint(QPainter::Antialiasing, true);//消锯齿
    QPen drawTrianglePen;//创建画笔

    drawTrianglePen.setColor(Qt::magenta);
    drawTrianglePen.setWidth(8);
    p.setPen(drawTrianglePen);
    p.drawPolygon(points, 3);//绘制箭头部分
    int offsetX = int(par*siny / 3);
    int offsetY = int(par*cosy / 3);
    QPoint point3, point4;
    point3 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)) + offsetX, endPoint.y() + int(-par*siny + (par / 2.0*cosy)) - offsetY);
    point4 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny) - offsetX), endPoint.y() - int(par / 2.0*cosy + par*siny) + offsetY);
    QPoint arrowBodyPoints[3] = { startPoint, point3, point4 };
    drawTrianglePen.setColor(Qt::cyan);
    p.setPen(drawTrianglePen);
    p.drawPolygon(arrowBodyPoints, 3);//绘制箭身部分
    qDebug()<<"我真的划线了";

}
int Board:: MobilityValues(int id)
{
//    6,  12,  6,  15, 0,  1,   1
    switch (_s[id]._type)
    {
    case Stone::CHE:
        return 6;

    case Stone::MA:
        return 12;

    case Stone::PAO:
        return 6;

    case Stone::BING:
        return 15;

    case Stone::JIANG:
        return 0;

    case Stone::SHI:
        return 1;

    case Stone::XIANG:
        return 1;

    }
    return 0;
}
void Board::keyReleaseEvent(QKeyEvent *ev)
{
    if(_PushKey)
    {
        if(ev->key()==Qt::Key_Left|ev->key()==Qt::Key_W)
        {
            qDebug()<<"我在按左按键";
            if(_stepsForRestoreCount>0)
            {
                reliveStone(_stepsForRestore.at(_stepsForRestoreCount-1)->_killid);
                moveStone(_stepsForRestore.at(_stepsForRestoreCount-1)->_moveid, _stepsForRestore.at(_stepsForRestoreCount-1)->_rowFrom, _stepsForRestore.at(_stepsForRestoreCount-1)->_colFrom);
                _stepsForRestoreCount--;
                qDebug()<<"我在后退";
            }
        }
        if(ev->key()==Qt::Key_Right|ev->key()==Qt::Key_D)
        {
            qDebug()<<"我在按右按键";
            if(_stepsForRestoreCount<_stepsForRestore.size())
            {
                killStone(_stepsForRestore.at(_stepsForRestoreCount)->_killid);
                moveStone(_stepsForRestore.at(_stepsForRestoreCount)->_moveid, _stepsForRestore.at(_stepsForRestoreCount)->_rowTo, _stepsForRestore.at(_stepsForRestoreCount)->_colTo);
                _stepsForRestoreCount++;
                qDebug()<<"我在前进";
            }
        }
        update();
    }
//    qDebug()<<QString::number(_stepsForRestoreCount);


}
void Board::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
       click(event->pos());
        if(_selectid>-1)
        {
            _row11=_s[_selectid]._row;
            _col11=_s[_selectid]._col;
        }

    }
}
void Board::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {

        if(_selectid==-1)
        {
            return;
        }
        else {

           int  x,y;
           getClickRowCol(event->pos(), x, y);
           _s[_selectid]._row=x;
           _s[_selectid]._col=y;
        }
        update();
    }

}
void Board::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(_selectid>-1)
        {
            _s[_selectid]._row=_row11;
            _s[_selectid]._col=_col11;
            click(event->pos());

        }
        update();
    }
}

