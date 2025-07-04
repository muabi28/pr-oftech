#ifndef GRAPFDIALOG_H
#define GRAPFDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class GrapfDialog;
}

/**
 * @class GrapfDialog
 * @brief Класс отображения графика
 * @details Класс отвечает за вычисление и отображние графика
 */
class GrapfDialog : public QDialog
{
    Q_OBJECT

signals:
    /**
     * @brief Сигнал о том, что окно с графиком закрыто.
     */
    void grapfClosed();

public:
    /**
     * @brief Метод, записывающий значение в словарь
     * @details Если ключ существует, то значение добавляется по ключу, иначе создается новое
     * @param manufacturer производитель - ключ
     * @param cost цена - значение
     */
    void insertIntoMap(QString manufacturer, double cost);

    /**
     * @brief Метод, полностью очищающий словарь
     */
    void dropMapData();


    /**
     * @brief Метод, устанавливающий новое имя для окна
     * @param parentPart строка - имя прявязанного основоного окна
     */
    void setNewWindowName(QString parentPart);

protected:
    /**
     * @brief Обработчик события закрытия окна.
     * @param event Событие закрытия окна.
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief Обработчик события отрисовки графика.
     * @details Вычисляет и полностью отрисовывает график на основе данных словаря
     * @param event Событие рисования в окне.
     */
    void paintEvent(QPaintEvent *event) override;

public:
    /// @brief Конструктор по умолчанию
    explicit GrapfDialog(QWidget *parent = nullptr);

    /// @brief Деструктор по умолчанию
    ~GrapfDialog();

private slots:
    /// @brief Слот, обрабатывающий нажатие на кнопку "закрыть"
    void on_closeButton_clicked();

    void on_pushButton_clicked();

private:
    /// @brief Ссылка на интерфейс окна
    Ui::GrapfDialog *ui;

    /// @brief Словарь для построения графика
    QMap<QString, double> data;

    /// @brief Хранит оригинальное название окна (Название в состоянии когда файл не открыт)
    QString orginalTitle = "";
};

#endif // GRAPFDIALOG_H
