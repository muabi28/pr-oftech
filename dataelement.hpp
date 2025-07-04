#ifndef DATAELEMENT_HPP
#define DATAELEMENT_HPP

#include <QString>

/**
 * @class DataElement
 * @brief Хранит информацию об отдельной записи в таблице
 */
class DataElement
{
private:
    /// @brief Хранит тип устройства
    QString deviceType;

    /// @brief Хранит категорию
    QString category;

    /// @brief Хранит производителя
    QString manufacturer;

    /// @brief Хранит модель
    QString deviceModel;

    /// @brief Хранит дату поступления
    QString arrivalDate;

    /// @brief Хранит цену
    double price;

    /// @brief Хранит местанахождение
    QString location;

    /// @brief Хранит скрок эксплуатации
    unsigned int yearsInUse;

public:
    /**
     *  @brief Конструктор по умолчанию
     *  @details Создает пусую запись
     */
    DataElement();

    /// @brief Конструктор с заранее определенными параметрами
    DataElement(QString &deviceType, QString &category, QString &manufacturer,
                             QString &deviceModel, QString &arrivalDate, double price,
                             QString &location, int yearsInUse);

    /// @brief Конструктор копирования
    DataElement(const DataElement &other);

    /// @brief Конструктор из строки
    DataElement(QString splitLine);

    /// @brief Перегрузка оператора присваивания
    DataElement &operator=(const DataElement &other);

    /**
     * @brief Метод для получаения информации о записи в формате строки
     * @return Строка c инфомацией о записи, столбцы разделены точкой с запятой
     */
    QString getSplitString();

    /// @brief Деструктор по умолчанию
    ~DataElement();

    /**
     * @brief Метод, устанавливающий тип устройства
     * @param deviceType тип устройства
     */
    void setdeviceType(const QString &deviceType);

    /**
     * @brief Метод, устанавливающий категорию устройства
     * @param category категория устройства
     */
    void setcategory(const QString &category);

    /**
     * @brief Метод, устанавливающий производителя устройства
     * @param manufacturer производитель устройства
     */
    void setmanufacturer(const QString &manufacturer);

    /**
     * @brief Метод, устанавливающий модель устройства
     * @param deviceModel модель устройства
     */
    void setdeviceModel(const QString &deviceModel);

    /**
     * @brief Метод, устанавливающий дату прибытия устройства
     * @param arrivalDate дата прибытия устройства
     */
    void setarrivalDate(const QString &arrivalDate);

    /**
     * @brief Метод, устанавливающий цену
     * @param price цена
     */
    void setprice(const double price);

    /**
     * @brief Метод, устанавливающий местонахождение устройства
     * @param location  местонахождение устройства
     */
    void setlocation(const QString &location);

    /**
     * @brief Метод, устанавливающий срок эксплуатации
     * @param yearsInUse срок эксплуатации
     */
    void setyearsInUse(const int yearsInUse);

    /// @brief Метод, возвращающий тип устройства
    QString getdeviceType() const;

    /// @brief Метод, возвращающий категорию
    QString getcategory() const;

    /// @brief Метод, возвращающий цену актива
    QString getmanufacturer() const;

    /// @brief Метод, возвращающий продавца актива
    QString getdeviceModel() const;

    /// @brief Метод, возвращающий контакты продавца актива
    QString getarrivalDate() const;

    /// @brief Метод, возвращающий статус актива
    double getprice() const;

    /// @brief Метод, возвращающий ответственного за покупку актива
    QString getlocation() const;

    /// @brief Метод, возвращающий источник финансирования актива
    int getyearsInUse() const;

    /// @brief Метод, говорящий, пустая ли запись
    bool isEmpty();
};
#endif // DATAELEMENT_HPP
