//维修历史管理
#ifndef SERVICE_HISTORY_H
#define SERVICE_HISTORY_H
#include<qstring.h>
class Service_History
{
public:
    Service_History(){

    }

    Service_History(QString Service_Time,QString Service_Engineer,QString Service_Result, QString Service_Price){
        strcpy(Service_time, Service_Time.toLocal8Bit().data());
        strcpy(Service_engineer, Service_Engineer.toLocal8Bit().data());
        strcpy(Service_result, Service_Result.toLocal8Bit().data());
        strcpy(Service_price, Service_Price.toLocal8Bit().data());
    }
    ~Service_History(){

    }
    QString getService_Time(){
        return  Service_time;
    }

    QString getService_Engineer(){
        return Service_engineer;
    }


    QString getService_Result(){
        return  Service_result;
    }

    QString getService_Price(){
        return Service_price;
    }



private:
    char Service_time[30];
    char Service_engineer[30];
    char Service_result[30];
    char Service_price[10];
;
};

#endif // SERVICE_HISTORY_H
