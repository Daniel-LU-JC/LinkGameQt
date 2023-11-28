#include "pushButton.h"
#include <QPropertyAnimation>
#include <QDebug>

void PushButton::zoom1() {
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(200);

    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation->setStartValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

PushButton::PushButton(QString normalImg, QString pressImg) {
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if(!ret) {
        qDebug() << "Loading Failure...";
        return;
    }

    this->setFixedSize(pix.width(), pix.height());
    this->setStyleSheet("QPushButton{border:0px}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(), pix.height()));
}
