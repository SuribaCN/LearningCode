#include "widget.h"
#include <QStandardItemModel>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QTableView(parent)
{
   QStandardItemModel* model = new QStandardItemModel(this);
   model->setItem(0, 0, new QStandardItem("张三"));
   model->setItem(0, 1, new QStandardItem("3"));
   model->setItem(0, 2, new QStandardItem("男"));
   this->setModel(model);
}

Widget::~Widget()
{

}
