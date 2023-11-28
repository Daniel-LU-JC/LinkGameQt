#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pushButton.h"
#include "singleMode.h"
#include "doubleMode.h"

#include <QTimer>
#include <QPainter>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QTextStream>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("QLink Game: by lujunchen");
    this->setFixedSize(1000, 900);
    this->setWindowIcon(QIcon(":/image/icon.png"));

    // 首先对于“单人模式”的按钮进行处理
    PushButton * singleBtn = new PushButton(":/image/single.png");
    singleBtn->setParent(this);
    singleBtn->move(this->width() * 0.5 - singleBtn->width() * 2, this->height() * 0.65);

    // 对于自定义地图尺寸的处理：创建新游戏之前提前输入，记录进入 txt 文件，并且载入时同时进行还原
    connect(singleBtn, &PushButton::clicked, [=] () {
        singleBtn->zoom1();
        QTimer::singleShot(100, this, [=] () {
            QDialog * dlg = new QDialog(this);
            dlg->resize(280, 250);
            dlg->setWindowTitle("选择地图尺寸");
            dlg->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

            // 输入地图 length 的提示性文字
            QLabel * label1 = new QLabel(dlg);
            label1->setText("地图的长：");
            label1->move(40, 50);

            // 输入地图 length 的输入框
            QSpinBox * length = new QSpinBox(dlg);
            length->move(150, 40);
            length->resize(80, 35);
            length->setMaximum(10);
            length->setMinimum(6);

            // 输入地图 width 的提示性文字
            QLabel * label2 = new QLabel(dlg);
            label2->setText("地图的宽：");
            label2->move(40, 120);

            // 输入地图 width 的输入框
            QSpinBox * width = new QSpinBox(dlg);
            width->move(150, 110);
            width->resize(80, 35);
            width->setMaximum(8);
            width->setMinimum(5);

            // 进入游戏状态
            QPushButton * btn = new QPushButton(dlg);
            btn->move(75, 180);
            btn->setText("进入游戏");

            dlg->show();

            // 设置“进入游戏”按钮的点击状态
            connect(btn, &QPushButton::clicked, [=] () {

                // 传入地图自定义长宽的参数
                singleMode * gamewindow1 = new singleMode(length->value(), width->value(), false);

                this->hide();
                dlg->hide();
                gamewindow1->show();
                gamewindow1->timer1->start(1000);
            });
        });
    });

    // 接下来是对于“双人模式”按钮的处理
    PushButton * doubleBtn = new PushButton(":/image/double.png");
    doubleBtn->setParent(this);
    doubleBtn->move(this->width() * 0.5 + singleBtn->width(), this->height() * 0.65);

    connect(doubleBtn, &QPushButton::clicked, [=] () {
        doubleBtn->zoom1();
        QTimer::singleShot(100, this, [=] () {
            QDialog * dlg = new QDialog(this);
            dlg->resize(280, 250);
            dlg->setWindowTitle("选择地图尺寸");
            dlg->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

            // 输入地图 length 的提示性文字
            QLabel * label1 = new QLabel(dlg);
            label1->setText("地图的长：");
            label1->move(40, 50);

            // 输入地图 length 的输入框
            QSpinBox * length = new QSpinBox(dlg);
            length->move(150, 40);
            length->resize(80, 35);
            length->setMaximum(10);
            length->setMinimum(6);

            // 输入地图 width 的提示性文字
            QLabel * label2 = new QLabel(dlg);
            label2->setText("地图的宽：");
            label2->move(40, 120);

            // 输入地图 width 的输入框
            QSpinBox * width = new QSpinBox(dlg);
            width->move(150, 110);
            width->resize(80, 35);
            width->setMaximum(8);
            width->setMinimum(5);

            // 进入游戏状态
            QPushButton * btn = new QPushButton(dlg);
            btn->move(75, 180);
            btn->setText("进入游戏");

            dlg->show();

            // 设置“进入游戏”按钮的点击状态
            connect(btn, &QPushButton::clicked, [=] () {

                // 传入地图自定义长宽的参数
                doubleMode * gamewindow2 = new doubleMode(length->value(), width->value(), false);

                this->hide();
                dlg->hide();
                gamewindow2->show();
                gamewindow2->timer1->start(1000);
            });
        });
    });

    // 载入按钮
    PushButton * loadBtn = new PushButton(":/image/load.png");
    loadBtn->setParent(this);
    loadBtn->move(this->width() * 0.5 - singleBtn->width() * 2, this->height() * 0.8);
    connect(loadBtn, &PushButton::clicked, [=] () {
        loadBtn->zoom1();
        QTimer::singleShot(100, this, [=] () {
            QDialog * dlg = new QDialog(this);
            dlg->resize(200, 200);
            dlg->setWindowTitle("载入游戏");
            dlg->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            QPushButton * btn1 = new QPushButton(dlg);
            QPushButton * btn2 = new QPushButton(dlg);
            btn1->move(40, 50);
            btn2->move(40, 120);
            btn1->setText("单人模式");
            btn2->setText("双人模式");
            dlg->show();

            connect(btn1, &QPushButton::clicked, [=] () {

                QFile file("C:/Users/lujunchen2020/Desktop/Reference/QLink/save/single_game_record.txt");

                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);

                    int length = in.readLine().toInt();
                    int width = in.readLine().toInt();

                    singleMode * gamewindow1 = new singleMode(length, width, false);

                    for (int var = 1; var <= 40; var = var + 2) {
                        QString num = in.readLine();
                        int number = num.toInt();
                        gamewindow1->p[(var-1)/2].ID_x = number;
                        num = in.readLine();
                        number = num.toInt();
                        gamewindow1->p[(var-1)/2].ID_y = number;
                    }

                    QString num = in.readLine();
                    int number = num.toInt();
                    gamewindow1->ones.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->ones.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->hint.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->hint.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->shuffle.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->shuffle.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->flash.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->flash.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->mark = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->time = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->x = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->length = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->width = number;

                    gamewindow1->show();
                    this->hide();
                    dlg->hide();
                }
            });

            // 下面是对于双人模式的复原
            connect(btn2, &QPushButton::clicked, [=] () {
                QFile file("C:/Users/lujunchen2020/Desktop/Reference/QLink/save/double_game_record.txt");

                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);

                    int length = in.readLine().toInt();
                    int width = in.readLine().toInt();

                    doubleMode * gamewindow1 = new doubleMode(length, width, false);

                    for (int var = 1; var <= 40; var = var + 2) {
                        QString num = in.readLine();
                        int number = num.toInt();
                        gamewindow1->p[(var-1)/2].ID_x = number;
                        num = in.readLine();
                        number = num.toInt();
                        gamewindow1->p[(var-1)/2].ID_y = number;
                    }

                    QString num = in.readLine();
                    int number = num.toInt();
                    gamewindow1->ones.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->ones.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->hint.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->hint.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->shuffle.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->shuffle.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->flash.ID_x = number;
                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->flash.ID_y = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->mark = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->time = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->x = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->y = number;

                    gamewindow1->mark2 = in.readLine().toInt();
                    gamewindow1->x2 = in.readLine().toInt();
                    gamewindow1->y2 = in.readLine().toInt();

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->length = number;

                    num = in.readLine();
                    number = num.toInt();
                    gamewindow1->width = number;

                    gamewindow1->show();
                    this->hide();
                    dlg->hide();
                }
            });
        });
    });

    // 最后是退出游戏按钮
    PushButton * closeBtn = new PushButton(":/image/exit.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width() * 0.5 + singleBtn->width(), this->height() * 0.8);

    connect(closeBtn, &PushButton::clicked, [=] () {
        closeBtn->zoom1();
        QTimer::singleShot(100, this, [=] () {
            this->close();
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QColor background(221, 187, 238, 191);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(background);
    painter.drawRect(rect());

    // 绘制 icon 以及游戏名称
    painter.drawPixmap(350, 75, 300, 300, QPixmap(":/image/icon.png"));
    painter.drawPixmap(300, 420, 400, 100, QPixmap(":/image/title.png"));
}
