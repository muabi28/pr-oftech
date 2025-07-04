#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
class FindDialog;
}
QT_END_NAMESPACE
/**
 * @class FindDialog
 * @brief Класс отображения интерфеса для поска
 * @details Класс отвечает отображение интерфеса поиска
 */
class FindDialog : public QDialog
{
    Q_OBJECT

signals:
    /**
     * @brief Сигнал, о том, что нужно произвести поиск
     * @param column столбец, в котором нужно произвести поиск
     * @param filter строка, фильтр для поиска
     */
    void findSignal(int column, const QString& filter);

    /// @brief Сигнал о закрытии окна поиска
    void findclosed();

protected:

    /**
     * @brief Обработчик события закрытия окна.
     * @param event Событие закрытия окна.
     */
    void closeEvent(QCloseEvent *event) override;

public:
    /// @brief Конструктор по умолчанию
    explicit FindDialog(QWidget *parent = nullptr);

    /// @brief Деструктор по умолчанию
    ~FindDialog();


    /**
     * @brief Метод, устанавливающий новое имя для окна
     * @param parentPart строка - имя прявязанного основоного окна
     */
    void setNewWindowName(QString parentPart);





private slots:
    /// @brief Слот, обрабатывающий нажатие на кнопку "закрыть"
    void on_closeButton_clicked();

    /**
     * @brief Слот, обрабатывающий изменение в строке для ввода теста поиска
     * @param arg1 тест в строке для поиска
     */
    void on_lineEdit_textChanged(const QString &arg1);


    /**
     * @brief Слот, обрабатывающий изменение в ComboBox выбора столбца сортироки
     * @param index Индекс выбранного столбца сортировки
     */
    void on_columnBox_currentIndexChanged(int index);



private:
    /// @brief Ссылка на интерфейс окна
    Ui::FindDialog *ui;

    /// @brief Хранит оригинальное название окна (Название в состоянии когда файл не открыт)
    QString orginalTitle = "";

};

#endif // FINDDIALOG_H
