#include "mainwindow.h"

#include <QApplication>
#include<getin.h>
#include<qstring.h>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Fuk U");    w.show();
    return a.exec();
}
