#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H
#include <QDialog>
#include"ChooseDlg.h"
#include <QWidget>
#include "Board.h"

class MainWindows : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindows(int gameType, QWidget *parent = 0);


    int _gameType;

signals:

public slots:
};

#endif // MAINWINDOWS_H
