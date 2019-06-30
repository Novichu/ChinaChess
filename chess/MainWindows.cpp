#include "MainWindows.h"
#include <QHBoxLayout>
#include "SingleGameN.h"
#include "SqliteDatabase.h"
#include "SingleVSsingle.h"
#include "Board.h"
#include "NetGame.h"

#include<QMessageBox>

MainWindows::MainWindows(int gameType, QWidget *parent) : QWidget(parent)
{
    _gameType = gameType;

    if(_gameType == 0)
    {
        SqliteDatabase *b =new SqliteDatabase;
        b->show();

    }
    else if(_gameType == 1)
    {
        Board* game = new Board;

        game->show();

    }
    else if(_gameType == 3)
    {
        NetGame* game = new NetGame(true);

        game->show();
    }
    else if(_gameType == 4)
    {
        NetGame* game = new NetGame(false);

        game->show();

    }
    else if(_gameType==2)
    {
        SingleVSsingle* game =new SingleVSsingle;

        game->show();
    }

}
