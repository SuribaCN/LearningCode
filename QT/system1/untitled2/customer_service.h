#ifndef CUSTOMER_SERVICE_H
#define CUSTOMER_SERVICE_H
#include<qstring.h>
class Customer_Service
{
public:
    Customer_Service(){

    }

    Customer_Service(QString Service_Customer,QString Service_Good,QString Predict_Service_Time, QString Service_Information){
        strcpy(Service_customer, Service_Customer.toLocal8Bit().data());
        strcpy(Service_good, Service_Good.toLocal8Bit().data());
        strcpy(Predict_service_time, Predict_Service_Time.toLocal8Bit().data());
        strcpy(Service_information, Service_Information.toLocal8Bit().data());
    }
    ~Customer_Service(){

    }
    QString getService_Customer(){
        return  Service_customer;
    }

    QString getService_Good(){
        return Service_good;
    }


    QString getPredict_Service_Time(){
        return  Predict_service_time;
    }

    QString getService_Information(){
        return Service_information;
    }



private:
    char Service_customer[30];
    char Service_good[30];
    char Predict_service_time[30];
    char Service_information[240];
};
#endif // GETIN_H

