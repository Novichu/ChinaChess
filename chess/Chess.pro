HEADERS += \
    Board.h \
    Stone.h \
    Step.h \
    SingleGameN.h \
    NetGame.h \
    MainWindows.h \
    ChooseDlg.h \
    SqliteDatabase.h \
    SingleVSsingle.h

SOURCES += \
    Board.cpp \
    main.cpp \
    Stone.cpp \
    Step.cpp \
    SingleGameN.cpp \
    NetGame.cpp \
    MainWindows.cpp \
    ChooseDlg.cpp \
    SqliteDatabase.cpp \
    SingleVSsingle.cpp

QT += widgets gui network sql

DISTFILES +=

INCLUDEPATH+= C:/Users/Administrator/AppData/Local/Programs/Python/Python37/include/

LIBS += C:/Users/Administrator/AppData/Local/Programs/Python/Python37/libs/python37.lib
