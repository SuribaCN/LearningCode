#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLabel>
#include <QMainWindow>
#include <QtWidgets>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    enum CellType{ctName=1000,ctSex,ctBirth,ctNation,ctPartyM,ctScore};
    enum FieldColNum{colName=0,colSex,colBirth,colNation,colScore,colPartyM};
    QLabel *labCellIndex;
    QLabel *labCellType;
    QLabel *labStudID;
    void createItemsARow(int eowNo,QString Name,QString Sex,QDate birth,QString Nation,bool isPM,int score);
};
class QLineEdit;
class QDialog;
#endif // MAINWINDOW_H
