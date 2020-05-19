#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include<QPushButton>
#include<QTextStream>
#include <QTextEdit>
#include<tool.h>
//商品信息管理
#include<getin.h>
//顾客信息管理
#include<customer.h>
//报修信息管理
#include<customer_service.h>
//维修历史管理
#include<service_history.h>

#define AddressOfStandardMessage "C:\\Users\\Suriba\\source\\repos\\QT\\system1\\untitled2\\debug\\StandardMessage.txt"
#define AddressOfCustomerMessage "C:\\Users\\Suriba\\source\\repos\\QT\\system1\\untitled2\\debug\\CustomerMessage.txt"
#define AddressOfServiceMessage "C:\\Users\\Suriba\\source\\repos\\QT\\system1\\untitled2\\debug\\ServiceMessage.txt"
#define AddressOfServiceHitoryMessage "C:\\Users\\Suriba\\source\\repos\\QT\\system1\\untitled2\\debug\\ServiceHistory.txt"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    labCellIndex = new QLabel("当前单元坐标格",this);
    labCellIndex ->setMinimumWidth(250);
    labCellType =new QLabel("当前单元格类型:",this);
    labCellType->setMinimumWidth(200);
    labStudID=new QLabel("ID",this);
    labStudID->setMinimumWidth(200);
    ui->statusbar->addWidget(labCellIndex);
    ui->statusbar->addWidget(labCellType);
    ui->statusbar->addWidget(labStudID);
    //奇偶行深浅色
     ui->tableWidget->setAlternatingRowColors(1);
     //读取数据

}

MainWindow::~MainWindow()
{
    delete ui;
}

//全局变量传递输出文件地址
QString TargetAddress=0;

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{

     QTableWidgetItem* item=ui->tableWidget->item(row,column); //获取单元格的 Item
     if  (item==NULL)
         return;

     labCellIndex->setText(QString::asprintf("当前单元格坐标：%d 行，%d 列",row,column));

     int cellType=item->type();//获取单元格的类型
     labCellType->setText(QString::asprintf("当前单元格类型：%d",cellType));

     item=ui->tableWidget->item(row,MainWindow::colName); //取当前行第1列的单元格的 item
     int ID=item->data(Qt::UserRole).toInt();//读取用户自定义数据
     labStudID->setText(QString::asprintf("ID：%d",ID));
}
//插入行
void MainWindow::on_pushButton_5_clicked()
{
    int curRow=ui->tableWidget->currentRow();//当前行号
        ui->tableWidget->insertRow(curRow); //插入一行
}
//添加行
void MainWindow::on_pushButton_4_clicked()
{

        //int curRow;
        int curRow=ui->tableWidget->rowCount();//当前行号
        ui->tableWidget->insertRow(curRow);//在表格尾部添加一行

}
//删除行
void MainWindow::on_pushButton_6_clicked()
{
    int curRow=ui->tableWidget->currentRow();//当前行号
    ui->tableWidget->removeRow(curRow); //删除当前行及其items
}

//基本信息管理
void MainWindow::on_pushButton_clicked()
{
    QVector<Product> vec;
    QFile file(AddressOfStandardMessage);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            QStringList list = stream.readLine().split(QRegExp("\\s+"));
            Product pro(list.at(0), list.at(1), static_cast<QString>(list.at(2)).toInt(), static_cast<QString>(list.at(3)).toShort());
            vec.push_back(pro);

        }
    }
    ui->tableWidget->setRowCount(vec.count());     //设置行数为2
       ui->tableWidget->setColumnCount(4);   //设置列数为4

       QStringList header;
       header << "商品名称" << "商品分类" << "商品价格" << "保修期限";
       ui->tableWidget->setHorizontalHeaderLabels(header);


       for(int i = 0; i < vec.count(); i++)
       {
           ui->tableWidget->setItem(i,0,new QTableWidgetItem(vec[i].getName()));
           ui->tableWidget->setItem(i,1,new QTableWidgetItem(vec[i].getClass()));
           ui->tableWidget->setItem(i,2,new QTableWidgetItem(vec[i].getPrice()));
           ui->tableWidget->setItem(i,3,new QTableWidgetItem(vec[i].getService()));
       }
       file.close();
     TargetAddress=AddressOfStandardMessage;
}
//顾客信息管理
void MainWindow::on_pushButton_2_clicked()
{

    QVector<Customer> vec;
    QFile file(AddressOfCustomerMessage);

    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            QStringList list = stream.readLine().split(QRegExp("\\s+"));
            Customer customer(list.at(0), list.at(1), list.at(2),list.at(3),list.at(4));
            vec.push_back(customer);

        }
    }
    ui->tableWidget->setRowCount(vec.count());     //设置行数
       ui->tableWidget->setColumnCount(5);   //设置列数为5

       QStringList header;
       header << "顾客姓名" << "联系方式" << "购买商品" <<"购入时间"<< "保修期限";
       ui->tableWidget->setHorizontalHeaderLabels(header);


       for(int i = 0; i < vec.count(); i++)
       {
           ui->tableWidget->setItem(i,0,new QTableWidgetItem(vec[i].getCustomer_Name()));
           ui->tableWidget->setItem(i,1,new QTableWidgetItem(vec[i].getCustomer_Telephone()));
           ui->tableWidget->setItem(i,2,new QTableWidgetItem(vec[i].getSellout_Good()));
           ui->tableWidget->setItem(i,3,new QTableWidgetItem(vec[i].getSellout_Time()));
           ui->tableWidget->setItem(i,4,new QTableWidgetItem(vec[i].getService_Upto_time()));
       }
     file.close();
     TargetAddress=AddressOfCustomerMessage;
}
//保修信息管理
void MainWindow::on_pushButton_7_clicked()
{
    QVector<Customer_Service> vec;
    QFile file(AddressOfServiceMessage);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec("GBK");
        while(!stream.atEnd())
        {
            QStringList list = stream.readLine().split(QRegExp("\\s+"));
            Customer_Service customer_service(list.at(0), list.at(1),list.at(2), list.at(3));
            vec.push_back(customer_service);

        }
    }
    ui->tableWidget->setRowCount(vec.count());     //设置行数
       ui->tableWidget->setColumnCount(4);   //设置列数

       QStringList header;
       header << "顾客姓名" << "报修产品"<<"预计时间"<<"报修内容" ;
       ui->tableWidget->setHorizontalHeaderLabels(header);


       for(int i = 0; i < vec.count(); i++)
       {
           ui->tableWidget->setItem(i,0,new QTableWidgetItem(vec[i].getService_Customer()));
           ui->tableWidget->setItem(i,1,new QTableWidgetItem(vec[i].getService_Good()));
           ui->tableWidget->setItem(i,2,new QTableWidgetItem(vec[i].getPredict_Service_Time()));
           ui->tableWidget->setItem(i,3,new QTableWidgetItem(vec[i].getService_Information()));
       }
       file.close();
       TargetAddress=AddressOfServiceMessage;
}
//维修历史管理
void MainWindow::on_pushButton_3_clicked()
{
    QVector<Service_History> vec;
    QFile file(AddressOfServiceHitoryMessage);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec("GB2312");
        while(!stream.atEnd())
        {
            QStringList list = stream.readLine().split(QRegExp("\\s+"));//split()通过指定分隔符对字符串进行分片，QRegExp字符串模式匹配，\\s+任意空白分隔符
            Service_History service_history(list.at(0), list.at(1),list.at(2), list.at(3));
            vec.push_back(service_history);

        }
    }
    ui->tableWidget->setRowCount(vec.count());     //设置行数
       ui->tableWidget->setColumnCount(4);   //设置列数

       QStringList header;
       header << "上门时间" << "修理人员"<<"修理结果"<<"修理价格" ;
       ui->tableWidget->setHorizontalHeaderLabels(header);


       for(int i = 0; i < vec.count(); i++)
       {
           ui->tableWidget->setItem(i,0,new QTableWidgetItem(vec[i].getService_Time()));
           ui->tableWidget->setItem(i,1,new QTableWidgetItem(vec[i].getService_Engineer()));
           ui->tableWidget->setItem(i,2,new QTableWidgetItem(vec[i].getService_Result()));
           ui->tableWidget->setItem(i,3,new QTableWidgetItem(vec[i].getService_Price()));
       }
        file.close();
        TargetAddress=AddressOfServiceHitoryMessage;
}
//保存更改
void MainWindow::on_pushButton_8_clicked()
{

    QFile file_data(TargetAddress);
        file_data.open(QIODevice::WriteOnly );
        file_data.close();
        file_data.open(QIODevice::ReadWrite);
        QString tabeDate[ui->tableWidget->rowCount()][ui->tableWidget->columnCount()];
         QTextStream in(&file_data);
        for(int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            for(int j=0; j<ui->tableWidget->columnCount(); j++)
            {
               in<<( tabeDate[i][j] = ui->tableWidget->item(i, j)->text())<<" ";
            }
            in<<"\r\n";
        }


        file_data.close();



}




