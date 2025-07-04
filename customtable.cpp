#include "customtable.hpp"
#include "dataelement.hpp"
#include "mainwindow.h"

#include <QMessageBox>

MyTableModel::MyTableModel(QObject *parent) : QAbstractTableModel(parent)
    , columnsNum(8) // Указываем, что столбцов будет восемь
{}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return DataModelList.count(); // считаем и возвращаем кол-во строк
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columnsNum; // Возвращаем кол-во столбцов
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant(); // Проверяем индекс на корректность

    // Получаем номер строки и столбца из индекса
    int row = index.row();
    int column = index.column();

    switch(role) // В зависимости от роли обрабатываем данные
    {
    // Обработка для случая DisplayRole
    case Qt::DisplayRole:
    // Так как, поведение для обоих случаев "DisplayRole" и "EditRole"
    // одинаковое, то и не надо ставить "brake" между ними и реализовывать
    // для каждой роли свой набор действий
    case Qt::EditRole:

        // В зависимости от столбца возвращаем нужные данные
        switch (column)
        {
        case 0: return DataModelList.at(row).getdeviceType();
        case 1: return DataModelList.at(row).getcategory();
        case 2: return DataModelList.at(row).getmanufacturer();
        case 3: return DataModelList.at(row).getdeviceModel();
        case 4: return DataModelList.at(row).getarrivalDate();
        case 5: return DataModelList.at(row).getprice();
        case 6: return DataModelList.at(row).getlocation();
        case 7: return DataModelList.at(row).getyearsInUse();
            break;
        }
    default:
        return QVariant(); // Если пришла какая-то другая роль, то возвращаем пустой QVariant
    }
}

bool MyTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole) // Обрабатываем только роль редактирования
    {
        layoutAboutToBeChanged(); // Сообщаем, что сейчас будут изменения в данных

        // Получаем номер строки и столбца из индекса
        int row = index.row();
        int column = index.column();

        switch (column) // В зависимости от номера столбца устанавливаем новые данные
        {
        case 0: DataModelList[row].setdeviceType(value.toString()); break;
        case 1: DataModelList[row].setcategory(value.toString()); break;
        case 2: DataModelList[row].setmanufacturer(value.toString()); break;
        case 3: DataModelList[row].setdeviceModel(value.toString()); break;
         case 4: // Проверяем формат ввода даты
            if (!value.toString().contains(QRegularExpression("^\\d{4}-\\d{2}-\\d{2}$"))) {
                QMessageBox::warning(nullptr, tr("Invalid Input"), tr("Date must be in the format yyyy-MM-dd."));
                return false; // Отклоняем некорректный ввод
            }
            DataModelList[row].setarrivalDate(value.toString());
            break;

        case 5: DataModelList[row].setprice(value.toDouble()); break;
        case 6: DataModelList[row].setlocation(value.toString()); break;
        case 7: // Проверяем формат ввода для столбца с yearsInUse (тип int)
            if (!value.toString().contains(QRegularExpression("^\\d+$"))) {
                QMessageBox::warning(nullptr, tr("Invalid Input"), tr("Years in use must contain only digits."));
                return false; // Отклоняем некорректный ввод
            }
            DataModelList[row].setyearsInUse(value.toInt());
            break;
        }

        emit modelDataChanged(); // Издаем сигнал, о том что данные в модели изменились
        layoutChanged(); // Сообщаем, что изменения сделаны

        return true; // Возвпращаем, что данные установлены успешно
    }
    return false; // Возвпращаем, что данные НЕ установлены
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Если ориентация - горизонтальная (заголовки столбцов) и роль отображения
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        switch (section) // В зависимости столбца возвращаем соответствующее название для заголовка
        {
        case 0: return QString(tr("Device Type"));
        case 1: return QString(tr("Category"));
        case 2: return QString(tr("Manufacturer"));
        case 3: return QString(tr("Model"));
        case 4: return QString(tr("Arrival Date"));
        case 5: return QString(tr("Price"));
        case 6: return QString(tr("Location"));
        case 7: return QString(tr("Years in use"));
        }
    return QVariant(); // Иначе, если ставим названия не для колонок или какая-то другая роль - возвращаем пустой QVariant
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    if(index.isValid()) // Если индекс корректный, то ...
        // ... возвращаем набор флагов что элемент
        return  Qt::ItemIsEnabled |  // Активный
               Qt::ItemIsSelectable | // Его можно выбрать
               Qt::ItemIsEditable | // Его можно редактировать

               // И что с ним можно делать Drag and Drop
               Qt::ItemIsDragEnabled |
               Qt::ItemIsDropEnabled;
    else
        return Qt::NoItemFlags; // Если индекс не корректный, возвращаем флаг, указывающий на отсутствие свойств у элемента
}

bool MyTableModel::appendRow(const DataElement &value)
{
    int position = rowCount(); // Мы будем вставлять в конец, поэтому получаем кол-во строк всего
    beginInsertRows(QModelIndex(), position, position); // Сообщаем, что сейчас будем вставлять строки и указываем что будем вставлять строку в самый конец
    DataModelList.append(value); // Добавляем строку в список данных модели
    endInsertRows(); // Сообщаем, что мы закончили вствку данных
    return true;
}

bool MyTableModel::removeRow(const int position)
{
    if (DataModelList.isEmpty()) return false; // Если список данных пуст, возвращаем false

    beginRemoveRows(QModelIndex(), position, position); // Сообщаем, что сейчас начнем удалять строку и указываем какую
    DataModelList.removeAt(position); // Удаляем нужную строку
    emit modelDataChanged(); // Издаем сигнал о том, что произошли изменения в данных
    endRemoveRows(); // Сообщаем, что мы закончили удаление данных
    return true;
}

DataElement MyTableModel::getDataFromTable(int row)
{
    return DataModelList.at(row); // Возвращаем даные строки
}

void MyTableModel::dropTable()
{
    int rows = rowCount(); // Получаем кол-во строк в
    for (int deletedrows = 0; deletedrows < rows; ++deletedrows) removeRow(0); // Пускаем цикл по строкам и удаляем все данные
    emit dataChanged(this->index(0, 0), this->index(0, 0)); // Издаем сигнал о том, что произошли изменения в данных
}

void MyTableModel::overWriteData(int row, const DataElement &value)
{
    layoutAboutToBeChanged(); // Сообщаем, что сейчас будут изменения в данных
    DataModelList.replace(row, value); // Перезаписываем нужную строку новыми данными
    emit dataChanged(this->index(0, 0), this->index(0, 0)); // Издаем сигнал о том, что произошли изменения в данных
    layoutChanged(); // Сообщаем, что произошли изменения в данных
}

QStringList MyTableModel::mimeTypes() const
{
    QStringList types; // Создание списка типов
    types << "application/x-qabstractitemmodeldatalist"; // Добавление внутреннего типа приложения
    types << "text/plain"; // Добавление текстового типа
    return types; // Возвращение списка типов
}

QMimeData* MyTableModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mimeData = new QMimeData; // Создание объекта мим-данных
    QString outText = ""; // Стока в которую будем переводить даннне исходной строки

    for (const QModelIndex& index : indexes) // Цикл по всем выделенным индексам
    {
        if (index.isValid()) // Если индекс корректен
        {
            QString text = this->data(index,Qt::DisplayRole).toString();  // Получение текста яцейки по индексу
            outText.append(text.toUtf8().append(";")); // Добавляем разделитель
        }
    }
    outText.append('\n'); // Добавляем перенос строки в выходную строку
    mimeData->setData("application/x-qabstractitemmodeldatalist", outText.toUtf8()); // Добавление текста в мим-данные в качестве внутренних данных
    mimeData->setData("text/plain", outText.toUtf8()); // Добавление строки в мим-данные в качестве текстового типа
    return mimeData;
}

bool MyTableModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{

    Q_UNUSED(action);
    Q_UNUSED(column);
    Q_UNUSED(row);

    if (data->hasFormat("application/x-qabstractitemmodeldatalist") ||
        data->hasFormat("text/plain")) // Если формат внутренний или текстовый
    {
        if (parent.isValid()) // И если индекс корректный
        {
            QString line = data->text(); // Получаем текст получаенных данных

            if(line.count(";") != 8) // Проверяем, то что строка корректна и содержит 8 столбцов
            {
                QMessageBox msgBox(QMessageBox::Warning,
                                   tr("Unsupported Input Data Format"),
                                   tr("Data format is not supported.\nYou can paste only rows."),
                                   QMessageBox::Ok);
                msgBox.exec(); return false; // Если нет - сообщаем пользователю и ничего не перезаписываем
            }

            this->overWriteData(parent.row(), DataElement(line)); // Перезаписываем строку, куда пользователь делает drop
            return true;
        }
    }
    return false;
}
