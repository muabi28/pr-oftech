#include "grapfdialog.h"
#include "ui_grapfdialog.h"

#include <QPainter>
#include <QCloseEvent>

GrapfDialog::GrapfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GrapfDialog)
{
    ui->setupUi(this);
    orginalTitle = this->windowTitle();
}

void GrapfDialog::closeEvent(QCloseEvent *event)
{
    emit grapfClosed(); // Издаем сигнал о том что график закрылся
    event->accept(); // Принимаем событие закрытия окна
}

GrapfDialog::~GrapfDialog()
{
    delete ui; // Освобождаем память от окна
}

void GrapfDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int indent = 100; // Задаем отступы от краев
    setFixedSize(data.count() * 80 + 2 * indent , 400); // Задаем размер окна, зависящего от кол-ва уникльных записей

    QPainter painter(this); // Создаем объект пэинтера, который будет рисовать на текущем окне

    int width = this->width(); // Ширина окна
    int height = this->height(); // ВЫсота окна

    int grapfBoxWidth = width - 2 * indent; // Ширина области для рисования с учетом отступов
    int grapfBoxHeight = height - 2 * indent; // Высота области для рисования с учетом отступом
    int grapfSegmentWidth = grapfBoxWidth / data.size(); // Ширина столбца графика

    // Ищем максимальное значение потраченной суммы
    double maxValue = 0;
    for (auto value : data.values()) // Перебор значений словаря
        if (value > maxValue)
            maxValue = value;

    QBrush brush; // Объект стиля закрашивания
    brush.setStyle(Qt::SolidPattern); // Устанавливаем сплошное закрашивание

    int columnIndex = 0;
    for (const auto& pair : data.toStdMap()) // Перебор по ключам и значениям
    {
        const QString& key = pair.first; // Ключ - год
        const int& value = pair.second; // Значение - сумма

        // Обеспечиваем чередование бирюзового и синего столбца графика
        if(columnIndex % 2 == 0) brush.setColor(Qt::cyan);
        else brush.setColor(Qt::blue);

        int barHeight = (float)value / maxValue * grapfBoxHeight; // Получаем высоту текущего стобца, относительно самого высокого столбца, выщетанного ранее

        // Отрисовываем столбец
        QRect barRect(indent + columnIndex * grapfSegmentWidth, // Координата верхней левой точкой по OX
                      indent + grapfBoxHeight - barHeight, // Координата верхней левой точкой по OY
                      grapfSegmentWidth - 2, // Длина
                      barHeight); // Высота

        painter.fillRect(barRect, brush); // Закрашиваем

        // Подписываем текущий столбец графика годом снизу
        painter.drawText(indent + columnIndex * grapfSegmentWidth,
                         height - indent + 5,
                         grapfSegmentWidth,
                         20,
                         Qt::AlignCenter,
                         key);

        // Подписываем текущий столбец графика ценой сверху
        painter.drawText(indent + columnIndex * grapfSegmentWidth,
                         indent + grapfBoxHeight - barHeight - 20,
                         grapfSegmentWidth,
                         20,
                         Qt::AlignCenter,
                         QString::number(value) + "k$");
        columnIndex++;
    }

    // Рисуем полосу под графиком, символизитрующую OX
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(indent - 1, height - indent, width - indent - 2, height - indent);

}

void GrapfDialog::insertIntoMap(QString manufacturer, double price)
{
    if(data.contains(manufacturer)) data[manufacturer] += price; // Если словарб содержит такой ключ, то плюсуем сумму
    else data[manufacturer] = price; // Иначе создаем новый ключ с исходным значением
}

void GrapfDialog::dropMapData()
{
    data.clear(); // Очищаем данные

}


void GrapfDialog::setNewWindowName(QString parentPart)
{
    // Формируем новое название окна
    QString windowname = orginalTitle + tr(" for ");
    windowname += parentPart;
    setWindowTitle(windowname);

}

void GrapfDialog::on_closeButton_clicked()
{
    this->close(); // Закрываем окно
}


void GrapfDialog::on_pushButton_clicked()
{
    this->close(); //Закрываем окно
}

