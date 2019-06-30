#ifndef NETGAME_H
#define NETGAME_H
#include<QPushButton>
#include"Board.h"
#include<QTcpServer>
#include<qtcpsocket.h>
class NetGame:public Board
{
    Q_OBJECT
public:
    NetGame(bool server);
    QTcpServer*_server;
    QTcpSocket*_socket;
    bool _again;
    void isServer(bool server);
    virtual void backStep();
    virtual void click(int id,int row,int col);
    int generateRandomInteger();
    void backFromNetwork(QByteArray buf);
    void drawFromNetwork(QByteArray buf);
    virtual void  getWin(int id);
    virtual void draw();//求和
    void thisinit();//初始化函数

public slots:
    void slotNewConnection();
    void slotRecv();
};

#endif // NETGAME_H
