#include "mainwindow.h"
#include "simpleTest.h"

#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}

// 用于进行自动化测试的程序入口
QTEST_MAIN(simpleTest)
