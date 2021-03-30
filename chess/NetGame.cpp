#include "NetGame.h"
#include <QTime>
#include<QMessageBox>
#include<cstdlib>
/*
    1.执红还是黑，服务器发出，客户端接收
    第一个字节固定1第二个字节1或者0；1表示接收方走红棋子，0表示走黑棋子
    2.点击信息
    第一个字节固定2，第二个字节是row，第三个col，第四个是点击棋子的id；
*/
NetGame::NetGame(bool server)
{
    _plte->hide();
    _restoreChess->hide();
    _server=NULL;
    _socket=NULL;
    _again=server;
    isServer(server);

}
void NetGame::isServer(bool server)
{
    if(server)
    {
        _server=new QTcpServer(this);
        _server->listen(QHostAddress::Any,9999);//9999端口
        connect(_server,SIGNAL(newConnection())
                ,this,SLOT(slotNewConnection()));
    }
    else {
        _socket=new QTcpSocket(this);
        _socket->connectToHost(QHostAddress("127.0.0.1"),9999);
        connect(_socket,SIGNAL(readyRead()),
                this,SLOT(slotRecv()));
    }
}

void NetGame::click(int id, int row, int col)
{
    if(_bRedTurn != _bSide)
        return;
    Board::click(id,row,col);
    //发送给对方
    char buf[4];
    buf[0]=2;
    buf[1]=9-row;//反转
    buf[2]=8-col;//
    buf[3]=id;
    _socket->write(buf,4);



}
void NetGame::slotRecv()
{
    QByteArray buf = _socket->readAll();
    switch (buf.at(0)) {
    case 1:
    {
        char data=buf[1];
        init(data==1);}

        break;
    case 2:
       { int row=buf[1];
        int col=buf[2];
        int id=buf[3];
        Board::click(id,row,col);}
        break;
    case 3:
        backFromNetwork(buf);
        break;
    case 4:
        drawFromNetwork(buf);
        break;
    case 5:
        thisinit();
        break;
    default:
        break;
    }
}
void NetGame::slotNewConnection()
{
    if(_socket)return;
    _socket= _server->nextPendingConnection();
    connect(_socket,SIGNAL(readyRead()),
            this,SLOT(slotRecv()));
     /*给对方发消息*/
    char buf[2];
    buf[0]=1;
    buf[1]=char(generateRandomInteger());//这个表示执红先行

    _socket->write(buf,2);
    init(buf[1]==0);
}
int NetGame:: generateRandomInteger()
{
    QTime time;
    time= QTime::currentTime();
    srand(time.msec()+time.second()*1000);
    int n = rand() % 2;    //产生2以内的随机数
    return n;
}
void NetGame::backFromNetwork(QByteArray)
{
    backOne();
    backOne();
    this->_plte->clear();
}
void NetGame::backStep()
{
    if(_bRedTurn != _bSide)
        return;
    backOne();
    backOne();

    QByteArray buf;
    buf.append(3);
    _socket->write(buf);
    this->_plte->clear();
}
void NetGame::drawFromNetwork(QByteArray)
{
    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(NULL,"GameOver","对方求和\n \n点击确认进入下一局比赛");
    if(ret==QMessageBox::Yes)
    {

            QByteArray buf;
            buf.append(5);
            _socket->write(buf);
            thisinit();

    }
}
void NetGame::draw()
{
    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(NULL,"GameOver","你向对方求和 \n \n如果对方确认则进入下一局比赛");
    if(ret==QMessageBox::Yes)
    {

            QByteArray buf;
            buf.append(4);
            _socket->write(buf);

    }

}
void NetGame::thisinit()
{
    this->_steps.clear();
    if(!_bSide)
    {
         _back->setStyleSheet("background-color:black;color:white");

    }else {

        _back->setStyleSheet("background-color:red;color:white");
    }
    for(int i=0; i<32; ++i)
    {
        _s[i].init(i);
    }
    if(_bSide)
    {
        for(int i=0; i<32; ++i)
        {
            _s[i].rotate();
        }
    }
}
void NetGame::getWin(int id)
{
    if (id==-1)
        return;
    if(_s[id]._type==Stone::JIANG&&_s[id]._dead==true)
    {

            if(_s[id]._red==_bRedTurn){


               QMessageBox::question(NULL,"GameOver","DEFEATED \n 点击进入下一局比赛");


            }
            else {
               QMessageBox::question(NULL,"GameOver","YOU WIN \n 点击进入下一局比赛");


            }
            thisinit();
    }
    this->_plte->clear();
}

