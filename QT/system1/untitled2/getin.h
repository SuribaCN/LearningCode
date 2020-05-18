#ifndef GETIN_H
#define GETIN_H
#include<qstring.h>
class Product
{
public:
    Product(){

    }

    Product(QString Product_Name, QString Product_Class, int Product_Price, short Product_Service){
        strcpy(Product_name, Product_Name.toLocal8Bit().data());
        strcpy(Product_class, Product_Class.toLocal8Bit().data());
        Product_price = Product_Price;
        Product_service = Product_Service;
    }
    ~Product(){

    }

    QString getName(){
        return Product_name;
    }

    QString getClass(){
        return Product_class;
    }

    QString getPrice(){
        return QString("%1").arg(Product_price);
    }

    QString getService(){
        return QString("%1").arg(Product_service);
    }


private:
    char Product_name[30];
    char Product_class[30];
    int Product_price;
    short Product_service;
};
#endif // GETIN_H
