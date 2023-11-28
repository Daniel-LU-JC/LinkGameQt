#ifndef DOUBLEMODE_H
#define DOUBLEMODE_H

#include <QWidget>
#include <QPoint>
#include <QBrush>
#include <QMouseEvent>
#include <QTimer>

#include "singleMode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class doubleMode; }
QT_END_NAMESPACE

class doubleMode : public QWidget
{
    Q_OBJECT

public:

    explicit doubleMode(int length, int width, bool is_loading, QWidget *parent = nullptr);
    ~doubleMode();

    void paintEvent(QPaintEvent *ev) override;
    void keyPressEvent(QKeyEvent *event) override;

    // 负责游戏刷新的两个计时器 QTimer
    QTimer * timer1 = new QTimer(this);
    QTimer * timer2 = new QTimer(this);

    int  x, y;  // 当前 RPG 角色的坐标
    int x2, y2;  // 第二个 RPG 角色的坐标

    point p[20];  // 全部的方块数组

    point first;
    int FIRST = -1;  // 第一个选中的点
    point second;
    int SECOND = -1;  // 第二个选中的点

    point one;
    int ONE = -1;  // 第一个选中的点
    point two;
    int TWO = -1;  // 第二个选中的点

    int s = 0, d = 0;  // 奇数方块点以及偶数方块点
    int S = 0, D = 0;

    bool isSame, isSame2;  // 选中的方块种类是否相同

    int time = 120;  // 游戏计时，总共 120 秒

    point ones;  // 延时 30s 的位置
    point hint;  // 提示 hint 的位置
    point shuffle;  // 打乱 shuffle 的位置
    point flash;  // flash 的位置

    point highlight1;  // 用来提示的第一个高光点
    point highlight2;  // 用来提示的第二个高光点

    void Tools();  // 调用所有道具的功能

    int mark = 0;  // 记录玩家当前的分数
    int mark2 = 0;  // 添加的第二个玩家当前的得分

    void save();  // 保存当前的游戏状态
    void load();  // 载入保存的游戏状态

    bool is_loading;  // 在游戏开始时是否需要载入保存的游戏状态

    bool straightLinkMatch(int x1, int y1, int x2, int y2);  // 两个点之间可以用直线直接相连
    bool inTwoLines(int x1, int y1, int x2, int y2);  // 需要通过两条相连的直线连接
    bool inThreeLines(int x1, int y1, int x2, int y2);  // 需要通过三条相连的直线连接

    bool isMore();  // 判断死局，是否存在更多的可以配对的方块

    // 两个方块能否用两折之内的线段连接
    bool LinkMatch(int x1, int y1, int x2, int y2);

    // 用来记录当前游戏地图“长”和“宽”的变量，取值范围是（10 * 8）~（6 * 5）
    int length, width;

private:

    Ui::doubleMode *ui;

signals:

};

#endif // DOUBLEMODE_H
