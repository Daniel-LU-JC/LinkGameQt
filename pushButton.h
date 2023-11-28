#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT

public:

    PushButton(QString normalImg, QString pressImg = "");

    QString normalImgPath;
    QString pressImgPath;

    void zoom1();

signals:

};

#endif // PUSHBUTTON_H
