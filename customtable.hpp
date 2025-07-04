#ifndef CUSTOMTABLE_HPP
#define CUSTOMTABLE_HPP

#include "dataelement.hpp"

#include <QAbstractTableModel>
#include <QMimeData>

/**
 * @class MyTableModel
 * @brief Собственная реализация класса QAbstractTableModel.
 * @details Отвечает за работу tableView.
 */
class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT

    /// @brief Кол-во столбцов
    const int columnsNum;

    /// @brief Список данных для строк в таблице
    QList<DataElement> DataModelList;

signals:
    /// @brief Сигнал о прошедших изменениях данных в таблице
    void modelDataChanged();

public:
    /// @brief Контструктор по умолчанию
    explicit MyTableModel(QObject *parent = nullptr);

    /// @brief Метод, возвращающий кол-во строк в модели
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /// @brief Метод, возвращающий кол-во столбиков в модели
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Метод, возвращающий обработанные данные для отображения в таблице
     * @param index Индекс ячейки в модели.
     * @param role Роль (отображение, редактирование и тд).
     * @return пустой QVariant, или данные для отображения
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief Метод, возвращающий набор флагов ячейки
     * @param index Индекс ячейки в модели.
     * @return Набор активных флагов (параметров) ячейки
     */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /**
     * @brief Метод, обновлящий данные в списке, после взаимодейсвтвия с ними через tableView
     * @param index Индекс ячейки в модели.
     * @param value значение, которое требуется установить
     * @param role Роль (отображение, редактирование и тд).
     * @return успешно ли прошла установка данных
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    /**
     * @brief Метод, возвращает сформированный заголовок
     * @param section столцец в таблице
     * @param orientation Ориентация (для определения строка/столбец)
     * @param role Роль (отображение, редактирование и тд). По-умолчанию Display
     * @return Сформированный заголовок
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    /**
     * @brief Метод, добавляющий новую строку в список данных.
     * @param value объект класса DataElement, по умполчанию пустой.
     * @return bool Запись добавлена успешно.
     */
    bool appendRow(const DataElement &value = DataElement());

    /**
     * @brief Метод, добавляющий удалающий строку из списка данных.
     * @param position номер удаляемой строки.
     * @return bool Запись удалена успешно.
     */
    bool removeRow(const int position);

    /**
     * @brief Метод для получаения записи из таблицы.
     * @return DataElement данные.
     */
    DataElement getDataFromTable(int);

    /// @brief Очищает таблицу.
    void dropTable();

    /**
     * @brief Метод, перезаписывающий данные в заданной строке.
     * @param row строка, в кототую нужно перезаписать данные.
     * @param value DataElement, на который нужео сделать замену.
     */
    void overWriteData(int row, const DataElement &value = DataElement());

    /**
     * @brief Метод, возвращающий поддерживающиеся типы mime данных
     * @return список поддерживающихся типы mime данных
     */
    QStringList mimeTypes() const override;

    /**
     * @brief Метод, формирующий создающий и формирующий mime данные при начале dragAndDrop
     * @param indexes Выделенные индексы, которые следует поместить в mime данные
     * @return Ссылка на объект QMimeData с обработанными данными
     */
    QMimeData* mimeData(const QModelIndexList& indexes) const override;

    /**
     * @brief Метод, обрабатывающий событие Drop (получения mime данных)
     * @param data Указатель на получаенные mime данные
     * @param action DropAction
     * @param row номер строки, куда производится drop
     * @param column номер столбца, куда производится drop
     * @param parent индекс, куда производится drop
     * @return bool успешно ли данные приняты
     */
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
};

#endif // CUSTOMTABLE_HPP
