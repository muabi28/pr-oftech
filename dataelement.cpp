#include "dataelement.hpp"
#include "math.h"

DataElement::DataElement():
    deviceType(),
    category(),
    manufacturer(),
    deviceModel(),
    arrivalDate(),
    price(),
    location(),
    yearsInUse()
{}

DataElement::DataElement(QString &deviceType, QString &category, QString &manufacturer,
                         QString &deviceModel, QString & arrivalDate, double price,
                         QString &location, int yearsInUse):
    deviceType(deviceType),
    category(category),
    manufacturer(manufacturer),
    deviceModel(deviceModel),
    arrivalDate(arrivalDate),
    price(price),
    location(location),
    yearsInUse(yearsInUse)
{}

DataElement::DataElement(const DataElement &other)
{
    deviceType = other.deviceType;
    category = other.category;
    manufacturer = other.manufacturer;
    deviceModel = other.deviceModel;
    arrivalDate = other.arrivalDate;
    price = other.price;
    location = other.location;
    yearsInUse = other.yearsInUse;
}

DataElement::DataElement(QString splitLine)
{
    deviceType = splitLine.mid(0, splitLine.indexOf(';')); splitLine.remove(0, splitLine.indexOf(';') + 1);
    category = splitLine.mid(0, splitLine.indexOf(';')); splitLine.remove(0, splitLine.indexOf(';') + 1);
    manufacturer = splitLine.mid(0, splitLine.indexOf(';')); splitLine.remove(0, splitLine.indexOf(';') + 1);
    deviceModel = splitLine.mid(0, splitLine.indexOf(';')); splitLine.remove(0, splitLine.indexOf(';') + 1);
    arrivalDate = splitLine.mid(0, splitLine.indexOf(';')); splitLine.remove(0, splitLine.indexOf(';') + 1);
    price = splitLine.mid(0, splitLine.indexOf(';')).toDouble(); splitLine.remove(0, splitLine.indexOf(';') + 1);
    location = splitLine.mid(0, splitLine.indexOf(';')); splitLine.remove(0, splitLine.indexOf(';') + 1);
    yearsInUse = splitLine.mid(0, splitLine.indexOf(';')).toInt(); splitLine.remove(0, splitLine.indexOf(';') + 1);
}

DataElement &DataElement::operator=(const DataElement &other)
{
    if (this == &other) return *this;

    deviceType = other.deviceType;
    category = other.category;
    manufacturer = other.manufacturer;
    deviceModel = other.deviceModel;
    arrivalDate = other.arrivalDate;
    price = other.price;
    location = other.location;
    yearsInUse = other.yearsInUse;

    return *this;
}

QString DataElement::getSplitString()
{
    QString splitString;

    splitString += this->getdeviceType() + ";";
    splitString += this ->getcategory() + ";";
    splitString += this ->getmanufacturer() + ";";
    splitString += this->getdeviceModel() + ";";
    splitString += this->getarrivalDate() + ";";
    splitString += QString::number(this->getprice()) + ";";
    splitString += this->getlocation() + ";";
    splitString += QString::number(this->getyearsInUse()) + ";";

    return splitString;
}

DataElement::~DataElement()
{}

void DataElement::setdeviceType(const QString &deviceType) {this->deviceType = deviceType;}
void DataElement::setcategory(const QString &category) {this->category = category;}
void DataElement::setmanufacturer(const QString &manufacturer) {this->manufacturer = manufacturer;}
void DataElement::setdeviceModel(const QString &deviceModel) {this->deviceModel = deviceModel;}
void DataElement::setarrivalDate(const QString &arrivalDate) {this->arrivalDate = arrivalDate;}
void DataElement::setprice(const double price) {this->price = price;}
void DataElement::setlocation(const QString &location) {this->location = location;}
void DataElement::setyearsInUse(const int yearsInUse) {this->yearsInUse = yearsInUse;}

QString DataElement::getdeviceType() const {return deviceType;}
QString DataElement::getcategory() const {return category;}
QString DataElement::getmanufacturer() const {return manufacturer;}
QString DataElement::getdeviceModel() const {return deviceModel;}
QString DataElement::getarrivalDate() const {return arrivalDate;}
double DataElement::getprice() const {return price;}
QString DataElement::getlocation() const {return location;}
int DataElement::getyearsInUse() const {return yearsInUse;}

bool DataElement::isEmpty()
{
    // Если хоть один элемент не пустой - позвращаем что весь экземпляр не пустой
    if(this->getdeviceType() != "") return false;
    if(this->getcategory() != "") return false;
    if(this->getmanufacturer() != "") return false;
    if(this->getdeviceModel() != "") return false;
    if(this->getprice() != 0) return false;
    if(this->getlocation() != "") return false;
    if(this->getarrivalDate() != "") return false;
    if(this->getyearsInUse() != 0) return false;
    return true;
}
