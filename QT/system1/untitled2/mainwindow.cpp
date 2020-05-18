#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QLabel>
#include <QSplitter>
#include <QDialog>
#include <QLineEdit>
#include<QPushButton>
#include<QMessageBox>
#include<QFileDialog>
#include<QTextStream>
#include<getin.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QVector<Product> vec;
    ui->setupUi(this);
    labCellIndex = new QLabel("当前单元坐标格",this);
    labCellIndex ->setMinimumWidth(250);
    labCellType =new QLabel("当前单元格类型:",this);
    labCellType->setMinimumWidth(200);
    labStudID=new QLabel("学生ID",this);
    labStudID->setMinimumWidth(200);
    ui->statusbar->addWidget(labCellIndex);
    ui->statusbar->addWidget(labCellType);
    ui->statusbar->addWidget(labStudID);
    //奇偶行深浅色
     ui->tableWidget->setAlternatingRowColors(1);
     //读取数据
     QFile file("aa.txt");
     if(file.open(QIODevice::ReadOnly))
     {
         QTextStream stream(&file);
         while(!stream.atEnd())
         {
             QStringList list = stream.readLine().split(QRegExp("\\s+"));
             Product pro(list.at(0), list.at(1), static_cast<QString>(list.at(2)).toInt(), static_cast<QString>(list.at(3)).toShort());
             vec.push_back(pro);

         }
     }
     ui->tableWidget->setRowCount(2);     //设置行数为2
        ui->tableWidget->setColumnCount(4);   //设置列数为4

        QStringList header;
        header << "Name" << "Flow" << "Distance" << "Time";
        ui->tableWidget->setHorizontalHeaderLabels(header);


        for(int i = 0; i < vec.count(); i++)
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(vec[i].getName()));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(vec[i].getClass()));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(vec[i].getPrice()));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(vec[i].getService()));
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{


     //设置表头
        QTableWidgetItem    *headerItem;
        QStringList headerText;
        headerText<<"姓 名"<<"性 别"<<"出生日期"<<"民 族"<<"分数"<<"是否党员";  //表头标题用QStringList来表示
        //ui->tableInfo->setHorizontalHeaderLabels(headerText);
        ui->tableWidget->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
        for (int i=0;i<ui->tableWidget->columnCount();i++)//列编号从0开始
        {
            //cellItem=ui->tableInfo->horizontalHeaderItem(i);
            headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
            QFont font=headerItem->font();//获取原有字体设置
            font.setBold(true);//设置为粗体
            font.setPointSize(12);//字体大小
            headerItem->setTextColor(Qt::red);//字体颜色
            headerItem->setFont(font);//设置字体
            ui->tableWidget->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
        }
         ui->tableWidget->setRowCount(5);
        ui->tableWidget->setVerticalHeaderLabels(QStringList()<<"1"<<"2"<<"3"<<"4"<<"5" );

}

void MainWindow::on_pushButton_2_clicked()
{
    QString strName,strSex;
        bool    isParty=false;
        QDate   birth;
        birth.setDate(1980,4,7);//初始化一个日期
        ui->tableWidget->clearContents();//只清除工作区，不清除表头
        int Rows=ui->tableWidget->rowCount(); //数据区行数，
        for (int i=0;i<Rows;i++) //数据区第1行的编号为0，所以范围是0~rowCount()-1
        {
            strName=QString::asprintf("学生%d",i); //学生姓名
            if ((i % 2)==0) //分奇数，偶数行设置性别，及其图标
                strSex="男";
            else
                strSex="女";
            createItemsARow(i, strName, strSex, birth,"汉族",isParty,70); //为某一行创建items
            birth=birth.addDays(20); //日期加20天
            isParty =!isParty;
        }
}



void MainWindow::createItemsARow(int rowNo,QString Name,QString Sex,QDate birth,QString Nation,bool isPM,int score)
{ //为一行的单元格创建 Items
    QTableWidgetItem    *item;
    QString str;
    uint StudID=201605000; //学号基数

    //姓名
    //新建一个Item，设置单元格type为自定义的MainWindow::ctName
    item=new  QTableWidgetItem(Name,MainWindow::ctName);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //文本对齐格式
    StudID  +=rowNo; //学号=基数+ 行号
    item->setData(Qt::UserRole,QVariant(StudID));  //设置studID为data
    ui->tableWidget->setItem(rowNo,MainWindow::colName,item); //为单元格设置Item

    //性别
    QIcon   icon;
    if (Sex=="男")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");
    item=new  QTableWidgetItem(Sex,MainWindow::ctSex); //新建一个Item，设置单元格type为自定义的 MainWindow::ctSex
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//为单元格设置Item
    ui->tableWidget->setItem(rowNo,MainWindow::colSex,item);//为单元格设置Item

    //出生日期
    str=birth.toString("yyyy-MM-dd"); //日期转换为字符串
    item=new  QTableWidgetItem(str,MainWindow::ctBirth);//新建一个Item，设置单元格type为自定义的 MainWindow::ctBirth
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); //文本对齐格式
    ui->tableWidget->setItem(rowNo,MainWindow::colBirth,item);//为单元格设置Item

    //民族
    item=new  QTableWidgetItem(Nation,MainWindow::ctNation); //新建一个Item，设置单元格type为自定义的 MainWindow::ctNation
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//文本对齐格式
    ui->tableWidget->setItem(rowNo,MainWindow::colNation,item);//为单元格设置Item

    //是否党员
    item=new  QTableWidgetItem("党员",MainWindow::ctPartyM);//新建一个Item，设置单元格type为自定义的 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//文本对齐格式
    if (isPM)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackgroundColor(Qt::yellow);//Qt::green  lightGray  yellow
    ui->tableWidget->setItem(rowNo,MainWindow::colPartyM,item);//为单元格设置Item

    //分数
    str.setNum(score);
    item=new  QTableWidgetItem(str,MainWindow::ctScore);//新建一个Item，设置单元格type为自定义的 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//文本对齐格式
    ui->tableWidget->setItem(rowNo,MainWindow::colScore,item);//为单元格设置Item
}




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
     labStudID->setText(QString::asprintf("学生ID：%d",ID));//学生ID
}

void MainWindow::on_pushButton_5_clicked()
{
    int curRow=ui->tableWidget->currentRow();//当前行号
        ui->tableWidget->insertRow(curRow); //插入一行，但不会自动为单元格创建item
        createItemsARow(curRow, "新学生", "男",
              QDate::fromString("1990-1-1","yyyy-M-d"),"苗族",true,60 ); //为某一行创建items
}

void MainWindow::on_pushButton_4_clicked()
{

        //int curRow;
        int curRow=ui->tableWidget->rowCount();//当前行号
        ui->tableWidget->insertRow(curRow);//在表格尾部添加一行
        createItemsARow(curRow, "新生", "女",
              QDate::fromString("2000-1-1","yyyy-M-d"),"满族",false,50 ); //为某一行创建items

}

void MainWindow::on_pushButton_6_clicked()
{
    int curRow=ui->tableWidget->currentRow();//当前行号
    ui->tableWidget->removeRow(curRow); //删除当前行及其items
}

void MainWindow::on_tableWidget_activated(const QModelIndex &index)
{
    ui->tableWidget->setAlternatingRowColors(1);
}


