#include <QApplication>
#include"Board.h"
//#include "SingleGame1.h"
//#include"SingleGame2.h"
#include<QMessageBox>
#
#include"ChooseDlg.h"
#include"SingleGameN.h"
//#include"NetGame.h"
#include"SqliteDatabase.h"
#include"MainWindows.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ChooseDlg dlg;
    dlg.setWindowOpacity(0.8);

    if(dlg.exec() != QDialog::Accepted)
        return 0;

    MainWindows wnd(dlg._selected);


//    SqliteDatabase b;
//    b.show();
    return app.exec();
}
