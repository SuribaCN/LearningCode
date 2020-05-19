#ifndef CUSTOMER_H
#define CUSTOMER_H
#include<qstring.h>
class Customer
{
public:
    Customer(){

    }

    Customer(QString Customer_Name, QString Customer_Telephone, QString Sellout_Good,QString Sellout_Time, QString Service_Upto_time){
        strcpy(Customer_name, Customer_Name.toLocal8Bit().data());
        strcpy(Customer_telephone, Customer_Telephone.toLocal8Bit().data());
         strcpy(Sellout_good, Sellout_Good.toLocal8Bit().data());
        strcpy(Sellout_time, Sellout_Time.toLocal8Bit().data());
        strcpy(Service_upto_time, Service_Upto_time.toLocal8Bit().data());
    }
    ~Customer(){

    }

    QString getCustomer_Name(){
        return  Customer_name;
    }

    QString getCustomer_Telephone(){
        return Customer_telephone;
    }
    QString getSellout_Good(){
    return Sellout_good;
    }
    QString getSellout_Time(){
        return Sellout_time;
    }

    QString getService_Upto_time(){
        return Service_upto_time;
    }


private:
    char Customer_name[30];
    char Customer_telephone[30];
    char Sellout_good[30];
    char Sellout_time[11];
    char Service_upto_time[11];
};
#endif // CUSTOMER_H
