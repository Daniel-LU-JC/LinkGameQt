#include "singleMode.h"
#include "ui_singleMode.h"
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QDebug>
#include <QBrush>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QPen>
#include <QFileDialog>

singleMode::singleMode(int length, int width, bool is_loading, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::singleMode)
{
    ui->setupUi(this);

    this->length = length;
    this->width = width;
    this->is_loading = is_loading;

    this->setFixedSize(1000, 900);
    this->setWindowTitle("QLink: 单人模式");
    this->setWindowIcon(QIcon(":/image/icon.png"));

    // 首先处理“暂停”按钮以及对应事件
    connect(ui->pause, &QPushButton::clicked, [=] ()
    {
        timer1->stop();

        QDialog * dialog = new QDialog(this);
        dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        dialog->setFixedSize(200, 200);
        dialog->setWindowTitle("游戏暂停");

        // 游戏暂停时可以保存游戏状态
        QPushButton * button1 = new QPushButton(dialog);
        button1->setText("保存");
        button1->move(40, 30);  // 注意设定按钮的位置

        connect(button1, &QPushButton::clicked, [=] ()
        {
            save();
            dialog->close();
            timer1->start(1000);
        });

        // 游戏暂停时可以载入之前的游戏状态
        QPushButton * button2 = new QPushButton(dialog);
        button2->setText("载入");
        button2->move(40, 80);

        connect(button2, &QPushButton::clicked, [=] ()
        {
            load();  // 将保存、载入等功能函数进行封装
            dialog->close();
            timer1->start(1000);
        });

        // 游戏暂停时可以返回当前的游戏中
        QPushButton * button3 = new QPushButton(dialog);
        button3->setText("返回");
        button3->move(40, 130);

        connect(button3, &QPushButton::clicked, [=] ()
        {
            timer1->start(1000);
            dialog->close();
        });

        dialog->exec();  // 弹出暂停对话框的定义
    });

    // 对于计时器 timer1 的行为进行绑定
    connect(timer1, &QTimer::timeout, [=] ()
    {
        if (time >= 0) {  // 游戏正在进行中，事件还没有消耗完
            QString num1 = QString::number(time / 60);
            ui->minute->setText(num1);
            QString num2 = QString::number(time % 60);
            ui->second->setText(num2);
            time = time - 1;
        } else {
            timer1->stop();
        }

        // 游戏超时结束
        if (time == 0) {
            QDialog * dialog2 = new QDialog(this);
            dialog2->setWindowTitle("游戏超时");
            dialog2->setFixedSize(200, 100);
            dialog2->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            QPushButton * button = new QPushButton(dialog2);
            button->setText("结束游戏");
            button->move(40, 30);
            connect(button, &QPushButton::clicked, dialog2, &QDialog::close);
            connect(button, &QPushButton::clicked, this, &QWidget::close);
            dialog2->exec();
        }

        // 判断是否全被消完，用 left 记录剩余的方块个数
        int left = 0;
        for (int var = 0; var < 20; ++var) {
            if(p[var].ID_x != 0 || p[var].ID_y != 0) {
                left = left + 1;
            }
        }

        if (left == 0 || !isMore()) {
            QDialog * dialog = new QDialog(this);
            dialog->setWindowTitle("游戏结束");
            dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            dialog->setFixedSize(200, 150);
            QLabel * label = new QLabel(dialog);
            label->setText("游戏胜利！");
            label->move(60, 30);
            QPushButton * button = new QPushButton(dialog);
            button->setText("关闭游戏");
            button->move(40, 80);
            connect(button, &QPushButton::clicked, dialog, &QDialog::close);
            connect(button, &QPushButton::clicked, this, &QWidget::close);
            dialog->exec();
        }
    });

    timer2->start(100);  // 游戏的刷新频率：0.1s
    connect(timer2, &QTimer::timeout, this, QOverload<>::of(&QWidget::repaint));
    connect(timer2, &QTimer::timeout, [=] ()
    {
        if (first.ID_x == 0 && first.ID_y == 0) {  // 如果 first 没有记录被选中的点，则优先用 first 记录
            for (int var = 0; var < 20; var++) {
                if(x == p[var].ID_x && y == p[var].ID_y) {
                    first.ID_x = x;
                    first.ID_y = y;
                    FIRST = var;  // 得到一个被激活的方块
                };
            };
        } else {
            if (first.ID_x != x || first.ID_y != y) {  // 如果想要激活另一个方块
                if (second.ID_x == 0 && second.ID_y == 0) {
                    for (int var = 0; var < 20; var++) {
                        if(x == p[var].ID_x && y == p[var].ID_y) {
                            second.ID_x = x;
                            second.ID_y = y;
                            SECOND = var;
                        };
                    };
                };
            };
        }

        Tools();  // 是否会触发道具的效果

        if (FIRST % 2 == 0 && SECOND % 2 == 1) {
            d = FIRST;
            s = SECOND;
        } else {
            isSame = false;
        }

        if (FIRST % 2 == 1 && SECOND % 2 == 0) {
            s=FIRST;
            d=SECOND;
        } else {
            isSame = false;
        }

        if (s == d + 1) {
            isSame = true;
        } else {
            isSame = false;
        }

        if (FIRST != -1 && SECOND != -1) {
            if (isSame && LinkMatch(p[FIRST].ID_x, p[FIRST].ID_y, p[SECOND].ID_x, p[SECOND].ID_y)) {
                p[FIRST].ID_x = 0;
                p[FIRST].ID_y = 0;
                p[SECOND].ID_x = 0;
                p[SECOND].ID_y = 0;  // 横纵坐标均为零意味着绘制时从屏幕中消失
                mark++;
                QString num = QString::number(mark);
                ui->scores->setText(num);
            }

            first.ID_x = 0;
            first.ID_y = 0;
            second.ID_x = 0;
            second.ID_y = 0;
            s = 0;
            d = 0;
            FIRST = -1;
            SECOND = -1;  // 将所有的辅助参数恢复到初始状态
        }
    });

    if (is_loading == false) {  // 在没有载入的情况下，需要随机生成各类方块的坐标
        qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);

        // 首先需要设置所有方块的坐标
        for (int var = 0; var < 20; ++var) {
            int j = 0;
            int x = qrand() % length;
            int y = qrand() % width;
            for (int i = 0; i < var; i++) {
                if(x + 1 == p[i].ID_x && y + 2 == p[i].ID_y) {  // 与已经存在的方块坐标发生重叠，需要被舍弃
                    j = j + 1;
                }
            }

            if (j == 0) {
                p[var].ID_x = x + 1;
                p[var].ID_y = y + 2;
            } else {
                var = var - 1;
            }
        }

        // 道具 hint 的坐标
        while(true) {
            int j = 0;
            int a = qrand() % length;
            int b = qrand() % width;
            for (int i = 0; i < 20; i++) {
                if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                    j = j + 1;
                }
            }

            if (j == 0) {
                hint.ID_x = a + 1;
                hint.ID_y = b + 2;
                break;
            }
        }

        // 道具 ones 的坐标
        while(true) {
            int j = 0;
            int a = qrand() % length;
            int b = qrand() % width;
            for (int i = 0; i < 20; i++) {
                if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                    j = j + 1;
                }
            }

            if (a + 1 == hint.ID_x && b + 2 == hint.ID_y) j++;

            if (j == 0) {
                ones.ID_x = a + 1;
                ones.ID_y = b + 2;
                break;
            }
        }

        // 道具 shuffle 的坐标
        while(true) {
            int j = 0;
            int a = qrand() % length;
            int b = qrand() % width;
            for (int i = 0; i < 20; i++) {
                if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                    j = j + 1;
                }
            }

            if (a + 1 == hint.ID_x && b + 2 == hint.ID_y) j++;
            if (a + 1 == ones.ID_x && b + 2 == ones.ID_y) j++;

            if (j == 0) {
                shuffle.ID_x = a + 1;
                shuffle.ID_y = b + 2;
                break;
            }
        }

        // 道具 flash 的坐标
        while(true) {
            int j = 0;
            int a = qrand() % length;
            int b = qrand() % width;
            for (int i = 0; i < 20; i++) {
                if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                    j = j + 1;
                }
            }

            if (a + 1 == hint.ID_x && b + 2 == hint.ID_y) j++;
            if (a + 1 == ones.ID_x && b + 2 == ones.ID_y) j++;
            if (a + 1 == shuffle.ID_x && b + 2 == shuffle.ID_y) j++;

            if (j == 0) {
                flash.ID_x = a + 1;
                flash.ID_y = b + 2;
                break;
            }
        }

        // 生成RPG角色坐标
        while (true) {
            qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
            int j = 0;
            int a = qrand() % length;
            int b = qrand() % width;
            for (int i = 0; i < 20; i++) {
                if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                    j = j + 1;
                }
            }
            if (a + 1 == hint.ID_x && b + 2 == hint.ID_y) j++;
            if (a + 1 == ones.ID_x && b + 2 == ones.ID_y) j++;
            if (a + 1 == shuffle.ID_x && b + 2 == shuffle.ID_y) j++;
            if (a + 1 == flash.ID_x && b + 2 == flash.ID_y) j++;

            if (j == 0) {
                x = a + 1;
                y = b + 2;
                break;
            }
        }
    }
}

// 键盘操作移动方向
void singleMode::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) {
        if (y >= 3 && y <= width + 1) {
            y = y - 1;
        } else if (y == 2) {
            y = width + 1;
        } else {
            y = y - 1;
        }
    }

    if (event->key() == Qt::Key_S) {
        if (y >= 2 && y <= width) {
            y = y + 1;
        } else if (y == width + 1) {
            y = 2;
        } else {
            y = y + 1;
        }
    }

    if (event->key() == Qt::Key_A) {
        if (x >= 2 && x <= length) {
            x = x - 1;
        } else if (x == 1) {
            x = length;
        } else {
            x = x - 1;
        }
    }

    if (event->key() == Qt::Key_D) {
        if (x >= 1 && x <= length - 1) {
            x = x + 1;
        } else if (x == length) {
            x = 1;
        } else {
            x = x + 1;
        }
    }
}

singleMode::~singleMode()
{
    delete ui;
}

void singleMode :: paintEvent(QPaintEvent *)
{
    // 首先绘制 RPG 角色
    QPainter player(this);
    QPen pen(QColor(255, 0, 0));
    player.setPen(pen);

    player.drawPixmap(x * 100 - 100, y * 100 - 100, 100, 100, QPixmap(":/image/character.png"));

    pen.setColor(QColor(255, 255, 255));
    player.setPen(pen);

    // 一次使用同样的颜色绘制两个方块
    for (int var = 0; var <= 20; var = var + 2) {
        qsrand(var);
        int r = qrand() % 25 * 10;
        int g = qrand() % 25 * 10;
        int b = qrand() % 25 * 10;
        QBrush brush(QColor(r, g, b, 130));
        player.setBrush(brush);  // 用于方块内部颜色的填充

        player.drawEllipse(QPoint((p[var].ID_x * 100 - 50), (p[var].ID_y * 100 - 50)), 50, 50);
        player.drawEllipse(QPoint((p[var+1].ID_x * 100 - 50), (p[var+1].ID_y * 100 - 50)), 50, 50);

        player.drawPixmap(p[var].ID_x * 100 - 100, p[var].ID_y * 100 - 100, 100, 100,
                          QPixmap(QString(":/image/pic0%1.png").arg(var / 2)));
        player.drawPixmap(p[var+1].ID_x * 100 - 100, p[var+1].ID_y * 100 - 100, 100, 100,
                QPixmap(QString(":/image/pic0%1.png").arg(var / 2)));
    }

    // 绘制所有的技能
    QBrush brush(QColor(255, 255, 255));
    player.setBrush(brush);
    player.drawEllipse(QPoint((ones.ID_x * 100 - 50), (ones.ID_y * 100 - 50)), 50, 50);
    player.drawPixmap(ones.ID_x * 100 - 100, ones.ID_y * 100 - 100, 100, 100,
                      QPixmap(":/image/tool3.png"));

    player.setBrush(brush);
    player.drawEllipse(QPoint((hint.ID_x * 100 - 50), (hint.ID_y * 100 - 50)), 50, 50);
    player.drawPixmap(hint.ID_x * 100 - 100, hint.ID_y * 100 - 100, 100, 100,
                      QPixmap(":/image/tool2.png"));

    player.setBrush(brush);
    player.drawEllipse(QPoint((shuffle.ID_x * 100 - 50), (shuffle.ID_y * 100 - 50)), 50, 50);
    player.drawPixmap(shuffle.ID_x * 100 - 100, shuffle.ID_y * 100 - 100, 100, 100,
                      QPixmap(":/image/tool1.png"));

    // highlight 用于提示信息的显示高亮
    QBrush brush1(QColor(255, 0, 0, 130));  // 在方块上方用红色透明的方式进行高亮
    player.setBrush(brush1);
    player.drawEllipse(QPoint((highlight1.ID_x * 100 - 50), (highlight1.ID_y * 100 - 50)), 50, 50);
    player.drawEllipse(QPoint((highlight2.ID_x * 100 - 50), (highlight2.ID_y * 100 - 50)), 50, 50);
}

// 该函数主要用于实现道具能够提供的功能
void singleMode::Tools()
{
    // +1s: 延长剩余时间 30s
    if (x == ones.ID_x && y == ones.ID_y) {
        time = time + 30;
        ones.ID_x=0;
        ones.ID_y=0;
    }

    // 10s 之内会高亮随意一对能够相连的方块，被消除之后高亮下一对，直至时间结束
    if (x == hint.ID_x && y == hint.ID_y) {
        hint.ID_x = 0;
        hint.ID_y = 0;

        QTimer * time = new QTimer(this);
        time->start(100);
        static int total = 0;
        connect(time, &QTimer::timeout, [=] ()
        {
            total++;
            if (total <= 100) {
                for (int var = 0; var <= 20; var = var + 2) {
                    // 在提示模式下，也需要注意对于两折之内可以连接的限制
                    if ((p[var].ID_x != 0 || p[var].ID_y != 0) &&
                            LinkMatch(p[var].ID_x, p[var].ID_y, p[var+1].ID_x, p[var+1].ID_y)) {
                        highlight1.ID_x = p[var].ID_x;
                        highlight1.ID_y = p[var].ID_y;
                        highlight2.ID_x = p[var+1].ID_x;
                        highlight2.ID_y = p[var+1].ID_y;
                        break;
                    }
                }
            } else {
                time->stop();
                highlight1.ID_x = 0;
                highlight1.ID_y = 0;
                highlight2.ID_x = 0;
                highlight2.ID_y = 0;  // 隐藏的方式就是将坐标全部置零
            }
        });
    }

    // shuffle: 所有方块位置重排
    if (x == shuffle.ID_x && y == shuffle.ID_y) {
        // 首先处理所有剩余的方块
        for (int var = 0; var < 20; ++var) {
            if (p[var].ID_x == 0 && p[var].ID_y == 0) continue;
            qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
            int j = 0;
            int x = qrand() % length;
            int y = qrand() % width;
            for (int i = 0; i < var; i++) {
                if (x + 1 == p[i].ID_x && y + 2 == p[i].ID_y) {
                    j = j + 1;
                }
            }
            if (j == 0) {
                p[var].ID_x= x + 1;
                p[var].ID_y= y + 2;
            } else {
                var = var - 1;
            }
        }

        shuffle.ID_x = 0;
        shuffle.ID_y = 0;

        // hint 坐标重排
        if (hint.ID_x != 0 || hint.ID_y != 0) {
            while (true) {
                int j = 0;
                int a = qrand() % length;
                int b = qrand() % width;
                for (int i = 0; i < 20; i++) {
                    if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                        j = j + 1;
                    }
                }
                if (j == 0) {
                    hint.ID_x = a + 1;
                    hint.ID_y = b + 2;
                    break;
                }
            }
        }

        // +30s 坐标重排
        if (ones.ID_x != 0 || ones.ID_y != 0) {
            while (true) {
                int j = 0;
                int a = qrand() % length;
                int b = qrand() % width;
                for (int i = 0; i < 20; i++) {
                    if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                        j = j + 1;
                    }
                }
                if (a + 1 == hint.ID_x && b + 2 == hint.ID_y) {
                    j++;
                }
                if (j == 0) {
                    ones.ID_x = a + 1;
                    ones.ID_y = b + 2;
                    break;
                }
            }
        }

        // flash 坐标重排
        if (flash.ID_x != 0 || flash.ID_y != 0) {
            while (true) {
                int j = 0;
                int a = qrand() % length;
                int b = qrand() % width;
                for (int i = 0; i < 20; i++) {
                    if (a + 1 == p[i].ID_x && b + 2 == p[i].ID_y) {
                        j = j + 1;
                    }
                }
                if (a + 1 == hint.ID_x && b + 2 == hint.ID_y) j++;
                if (a + 1 == ones.ID_x && b + 2 == ones.ID_y) j++;
                if (j == 0) {
                    flash.ID_x = a + 1;
                    flash.ID_y = b + 2;
                    break;
                }
            }
        }
    }
}

// 判断两个方块能否直接通过直线连接
bool singleMode::straightLinkMatch(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2) return false;

    bool okFlag = true;  // true 表示两个方块可以通过直线连接
    int max, min;

    if (x1 == x2) {
        if (y1 > y2) {
            max = y1;
            min = y2;
        } else {
            max = y2;
            min = y1;
        }

        for (int i = 0; i < 20; ++i) {
            if (p[i].ID_x == x1 && p[i].ID_y > min && p[i].ID_y < max) {
                okFlag = false;
            }
        }

        return okFlag;
    }

    if (y1 == y2) {
        if (x1 > x2) {
            max = x1;
            min = x2;
        } else {
            max = x2;
            min = x1;
        }

        for (int i = 0; i < 20; ++i) {
            if (p[i].ID_y == y1 && p[i].ID_x > min && p[i].ID_x < max) {
                okFlag = false;
            }
        }

        return okFlag;
    }

    return false;
}

// 判断两个方块能否通过仅有一折的折线连接
bool singleMode::inTwoLines(int x1, int y1, int x2, int y2) {
    // 矩形的两条边上没有方块阻挡
    bool flag1 = straightLinkMatch(x1, y1, x1, y2) && straightLinkMatch(x1, y2, x2, y2);
    bool flag2 = straightLinkMatch(x1, y1, x2, y1) && straightLinkMatch(x2, y1, x2, y2);

    // 矩形另外的两个顶点上是否有方块阻挡，true 代表没有阻挡
    bool flag3 = true;
    for (int var = 0; var < 20; ++var) {
        if (p[var].ID_x == x1 && p[var].ID_y == y2) {
            flag3 = false;
        }
    }

    bool flag4 = true;
    for (int var = 0; var < 20; ++var) {
        if (p[var].ID_x == x2 && p[var].ID_y == y1) {
            flag4 = false;
        }
    }

    // 既要判断矩形的两条边可以直接相连，又要保证对角上没有方块
    return (flag1 && flag3) || (flag2 && flag4);
}

// 判断两个方块能否通过两折的折线连接
bool singleMode::inThreeLines(int x1, int y1, int x2, int y2) {
    // 以（x1, y1）为中心，考察同一行/同一列可以直接到达的所有点，与（x2, y2）调用 inTwoLines 函数
    bool flag = false;

    // 首先在水平方向延申
    for (int i = 1; i <= length; ++i) {
        if (i == x1) continue;
        bool isEmpty = true;
        for (int j = 0; j < 20; ++j) {
            if (p[j].ID_x == i && p[j].ID_y == y1) {
                isEmpty = false;
            }
        }
        if (isEmpty) {
            flag = straightLinkMatch(i, y1, x1, y1) && inTwoLines(i, y1, x2, y2);
        }
        if (flag) return true;
    }

    // 接着在垂直方向延申
    for (int i = 2; i <= width + 1; ++i) {
        if (i == y1) continue;
        bool isEmpty = true;
        for (int j = 0; j < 20; ++j) {
            if (p[j].ID_x == x1 && p[j].ID_y == i) {
                isEmpty = false;
            }
        }
        if (isEmpty) {
            flag = straightLinkMatch(x1, i, x1, y1) && inTwoLines(x1, i, x2, y2);
        }
        if (flag) return true;
    }

    return false;
}

// 判断两个方块能否相消，也就是通过两次以内的折线进行连接
bool singleMode::LinkMatch(int x1, int y1, int x2, int y2)
{
    return straightLinkMatch(x1, y1, x2, y2) || inTwoLines(x1, y1, x2, y2) || inThreeLines(x1, y1, x2, y2);
}

// 最后还需要判断死局作为游戏结束的另一个指标，如果返回值是 false 则立即停止游戏并获得胜利
bool singleMode::isMore() {
    bool flag = false;
    for (int i = 0; i < 20; i += 2) {
        if (p[i].ID_x == 0 && p[i].ID_y == 0) continue;
        if (LinkMatch(p[i].ID_x, p[i].ID_y, p[i+1].ID_x, p[i+1].ID_y)) {
            flag = true;
            break;
        }
    }
    return flag;
}

// 下面的函数提供游戏的“存档”功能
void singleMode::save()
{
    QFile file("C:/Users/lujunchen2020/Desktop/Reference/QLink/save/single_game_record.txt");

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        QString num_x, num_y;

        num_x = QString::number(length);
        stream << num_x << endl;
        num_y = QString::number(width);
        stream << num_y << endl;

        // 首先记录所有方块的坐标
        for (int var = 0; var < 20; var++) {
            num_x = QString::number(p[var].ID_x);
            stream << num_x << endl;
            num_y = QString::number(p[var].ID_y);
            stream << num_y << endl;
        }

        num_x = QString::number(ones.ID_x);
        stream << num_x << endl;
        num_y = QString::number(ones.ID_y);
        stream << num_y << endl;

        num_x = QString::number(hint.ID_x);
        stream << num_x << endl;
        num_y = QString::number(hint.ID_y);
        stream << num_y << endl;

        num_x = QString::number(shuffle.ID_x);
        stream << num_x << endl;
        num_y = QString::number(shuffle.ID_y);
        stream << num_y << endl;

        num_x = QString::number(flash.ID_x);
        stream << num_x << endl;
        num_y = QString::number(flash.ID_y);
        stream << num_y << endl;

        num_x = QString::number(mark);
        stream << num_x << endl;
        num_y = QString::number(time);
        stream << num_y << endl;

        num_x = QString::number(x);
        stream << num_x << endl;
        num_y = QString::number(y);
        stream << num_y << endl;

        num_x = QString::number(length);
        stream << num_x << endl;
        num_y = QString::number(width);
        stream << num_y << endl;
    }

    file.close();
}

// 下面的函数提供游戏的“载入”功能
void singleMode::load()
{
    QFile file("C:/Users/lujunchen2020/Desktop/Reference/QLink/save/single_game_record.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QTextStream in(&file);

        // 重复对于 length 与 width 进行存储
        length = in.readLine().toInt();
        width = in.readLine().toInt();

        for (int var = 1; var <= 40; var = var + 2) {
            QString num = in.readLine();
            int number = num.toInt();
            p[(var-1)/2].ID_x = number;
            num = in.readLine();
            number = num.toInt();
            p[(var-1)/2].ID_y = number;
        }

        QString num = in.readLine();
        int number = num.toInt();
        ones.ID_x = number;
        num = in.readLine();
        number = num.toInt();
        ones.ID_y = number;

        num = in.readLine();
        number = num.toInt();
        hint.ID_x = number;
        num = in.readLine();
        number = num.toInt();
        hint.ID_y = number;

        num = in.readLine();
        number = num.toInt();
        shuffle.ID_x = number;
        num = in.readLine();
        number = num.toInt();
        shuffle.ID_y = number;

        num = in.readLine();
        number = num.toInt();
        flash.ID_x = number;
        num = in.readLine();
        number = num.toInt();
        flash.ID_y = number;

        num = in.readLine();
        number = num.toInt();
        mark = number;

        num = in.readLine();
        number = num.toInt();
        time = number;

        num = in.readLine();
        number = num.toInt();
        x = number;

        num = in.readLine();
        number = num.toInt();
        y = number;

        num = in.readLine();
        number = num.toInt();
        length = number;

        num = in.readLine();
        number = num.toInt();
        width = number;
    }

    first.ID_x = 0;
    first.ID_y = 0;
    second.ID_x = 0;
    second.ID_y = 0;
    s = 0;
    d = 0;
    FIRST = -1;
    SECOND = -1;  // 将所有的辅助参数恢复到初始状态

    is_loading = true;
}
