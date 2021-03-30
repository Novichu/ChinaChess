#include "ChooseDlg.h"
#include <QVBoxLayout>
#include <QPlainTextEdit>
ChooseDlg::ChooseDlg(QWidget *parent) : QDialog(parent)
{
    this-> setMinimumSize(1066,600);
    //背景图片
    QPixmap pixmap = QPixmap("2.png").scaled(this->size());

    QPalette palette(this->palette());
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QPlainTextEdit *_inforFace=new QPlainTextEdit();

    _inforFace->setReadOnly(true);

//    QTextOption :: QTextOption(Qt :: AlignCenter);
//    _inforFace->setWordWrapMode();
//    _inforFace->actions(tr("中国象棋"));
    _inforFace->clear();
    _inforFace->setWindowOpacity(0.2);
    _inforFace->setParent(this);
     _inforFace ->move(20*21+10,80*1);
     _inforFace->setFixedSize(550,30*14);

     _inforFace->appendPlainText("             中国象棋");
     _inforFace->setFont(QFont("Times", 18, QFont::Bold));
//     _inforFace->setStyleSheet("background-image:url(3.jpg)");

     _inforFace->appendPlainText("   中国象棋使用方形格状棋盘及红黑二色圆形棋子进行对弈，棋盘上有十条横线、九条竖线共分成90个交叉点；中国象棋的棋子共有32个，每种颜色16个棋子，分为7个兵种，摆放和活动在交叉点上。双方交替行棋，先把对方的将（帅）“将死”的一方获胜（因为擒贼先擒王）。已有几千年历史、充满东方智慧的中国象棋在中国的群众中基础远远超过围棋，一直是普及最广的棋类项目。");
//     _inforFace->setFont(QFont("Times", 15, QFont::Bold));
     _inforFace->show();

    //按钮列表
    QVBoxLayout* lay = new QVBoxLayout(this);

    lay->addWidget(_buttons[0] = new QPushButton("人机对战"));
    lay->addWidget(_buttons[1] = new QPushButton("人人对战"));
    lay->addWidget(_buttons[2] = new QPushButton("机机对战"));
    lay->addWidget(_buttons[3] = new QPushButton("网络对战(服务端)"));
    lay->addWidget(_buttons[4] = new QPushButton("网络对战(客户端)"));//


    for(int i=0; i<5; ++i)
    {
        _buttons[i]->setFixedSize(350,100);
        _buttons[i]->setWindowOpacity(0.5);
        _buttons[i]->move(80,50);
//       _buttons[i]->setFlat(true);
        _buttons[i]->setFont(QFont("Times", 20, QFont::Bold));
        connect(_buttons[i], SIGNAL(clicked()), this, SLOT(slotClicked()));
    }


}

void ChooseDlg::slotClicked()
{
    QObject* s = sender();
    for(int i=0; i<5; ++i)
    {
        if(_buttons[i] == s)
        {
            this->_selected = i;
            break;
        }
    }

    accept();

}

ChooseDlg::~ChooseDlg()
{
      this->close();
}

