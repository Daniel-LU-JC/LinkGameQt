#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <QObject>
#include <QtTest/QtTest>

class simpleTest : public QObject
{
    Q_OBJECT

private slots:
    void case1_testcase();

};

#endif // SIMPLETEST_H
