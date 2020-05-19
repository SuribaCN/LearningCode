#include "mainwindow.h"

#include <QApplication>
#include<getin.h>
#include<qstring.h>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("家电售后服务管理系统 V1.0 景 173401040114");
    w.show();
    return a.exec();
}
