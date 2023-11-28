#include "simpleTest.h"
#include "singleMode.h"

void simpleTest::case1_testcase() {
    singleMode * gamewindow = new singleMode(10, 8, false);
    gamewindow->p[0].ID_x = 1;
    gamewindow->p[0].ID_y = 3;
    gamewindow->p[1].ID_x = 6;
    gamewindow->p[1].ID_y = 4;
    gamewindow->p[2].ID_x = 1;
    gamewindow->p[2].ID_y = 8;
    gamewindow->p[3].ID_x = 5;
    gamewindow->p[3].ID_y = 4;
    gamewindow->p[4].ID_x = 5;
    gamewindow->p[4].ID_y = 5;
    gamewindow->p[5].ID_x = 6;
    gamewindow->p[5].ID_y = 2;
    gamewindow->p[6].ID_x = 8;
    gamewindow->p[6].ID_y = 8;
    gamewindow->p[7].ID_x = 5;
    gamewindow->p[7].ID_y = 7;
    gamewindow->p[8].ID_x = 4;
    gamewindow->p[8].ID_y = 9;
    gamewindow->p[9].ID_x = 1;
    gamewindow->p[9].ID_y = 9;
    gamewindow->p[10].ID_x = 7;
    gamewindow->p[10].ID_y = 5;
    gamewindow->p[11].ID_x = 10;
    gamewindow->p[11].ID_y = 5;
    gamewindow->p[12].ID_x = 1;
    gamewindow->p[12].ID_y = 7;
    gamewindow->p[13].ID_x = 2;
    gamewindow->p[13].ID_y = 2;
    gamewindow->p[14].ID_x = 4;
    gamewindow->p[14].ID_y = 5;
    gamewindow->p[15].ID_x = 2;
    gamewindow->p[15].ID_y = 4;
    gamewindow->p[16].ID_x = 9;
    gamewindow->p[16].ID_y = 4;
    gamewindow->p[17].ID_x = 7;
    gamewindow->p[17].ID_y = 7;
    gamewindow->p[18].ID_x = 3;
    gamewindow->p[18].ID_y = 8;
    gamewindow->p[19].ID_x = 6;
    gamewindow->p[19].ID_y = 8;  // 首先对于地图进行初始化

    QVERIFY(gamewindow->straightLinkMatch(3, 8, 6, 8));
    QVERIFY(gamewindow->straightLinkMatch(7, 5, 10, 5));
    QVERIFY(gamewindow->straightLinkMatch(4, 9, 1, 9));
    QVERIFY(gamewindow->inTwoLines(4, 5, 2, 4));
    QVERIFY(gamewindow->inTwoLines(9, 4, 7, 7));
    QVERIFY(gamewindow->inTwoLines(1, 3, 6, 4));
    QVERIFY(gamewindow->inThreeLines(8, 8, 5, 7));
    QVERIFY(gamewindow->inThreeLines(1, 7, 2, 2));
    QVERIFY(!gamewindow->LinkMatch(1, 8, 5, 4));
    QVERIFY(!gamewindow->LinkMatch(5, 5, 6, 2));  // 对于十种水果的连接状况进行测试
}
