#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customtable.hpp"
#include "grapfdialog.h"
#include "finddialog.h"

#include <QTranslator>
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QSettings>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
  * @class MainWindow
  * @brief Собственная реализация класса QMainWindow.
  * @details Отвечает за работу главного окна приложения.
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// @brief Конструктор по умолчанию
    MainWindow(QWidget *parent = nullptr);

    /// @brief Деструктор по умолчанию
    ~MainWindow();

    /**
      * @brief Метод setNewWindowTitle
      * @details Устанавливает новое имя окна в зависимости от открытого файла и факта его измененности
      */
    void setNewWindowTitle();

    /// @brief Хранит оригинальное название окна (Название в состоянии когда файл не открыт)
    QString originalTitle = "";

protected:
    /**
     * @brief Обработчик события закрытия окна.
     * @details Просит подтверждение перед закрытием окна и предлагаем сохранить файл, если были изменения
     * @param event Событие закрытия окна.
     */
    void closeEvent(QCloseEvent *event) override;



private slots:
    ///@brief Слот-обработчик создания нового файла в приложении.
    void on_actionNew_triggered();

    ///@brief Слот-обработчик открытия существующего файла в приложении.
    void on_actionOpen_triggered();

    ///@brief Слот-обработчик закрытия открытого в приложении файла.
    void on_actionClose_triggered();

    ///@brief Слот-обработчик выхода из приложения.
    void on_actionExit_triggered();

    ///@brief Слот-обработчик сохранения файла по открытому пути.
    void on_actionSave_triggered();

    ///@brief Слот-обработчик сохранения файла по выбранному пользователем пути.
    void on_actionSave_as_triggered();

    ///@brief Слот-обработчик добавления новой строки в таблицу.
    void on_actionAdd_row_triggered();

    ///@brief Слот-обработчик удаления строки из таблицы.
    void on_actionRemove_row_triggered();

    ///@brief Слот-обработчик фильтрации(поиска) по таблице.
    void on_actionFind_triggered();

    ///@brief Слот-обработчик копирования выделенной строки в буфер обмена.
    void on_actionCopy_row_triggered();

    ///@brief Слот-обработчик вырезания выделенной строки в буфер обмена.
    void on_actionCut_row_triggered();

    ///@brief Слот-обработчик вставки строки в таблицу.
    void on_actionPaste_row_triggered();

    /**
     * @brief Слот-обработчик отображения контекстного меню.
     * @param pos Координаты, где будет показано контекстное меню.
    */
    void showCustomContextMenu(const QPoint& pos);

    ///@brief Слот-обработчик вызова окна "Об авторе".
    void on_actionAuthor_triggered();


    ///@brief Слот-обработчик отмены сортировки.
    void on_actionSortDeny_triggered();

    /**
     * @brief Слот-обработчик запроса фильтрации(поиска) по таблице
     * @param column Столбец, по которому необходимо осуществить фильтрацию
     * @param filter регулярное выражение, применяемое для фильтрации
    */
    void findHandler(int column, const QString& filter);

    /**
     * @brief Слот-обработчик закрытия окна поиска.
     * @param Отменяет активную фильтрацию.
    */
    void findCloseHandler();

    /// @brief Слот-обработчик открытия нового главного окна приложения.
    void on_actionNew_window_triggered();

    /**
    * @brief Слот-обработчик изменения данных в модели.
    * @param Меняет имя окна в состояние, что файл изменен, если открыт график, обновляет его и устанавливает значение, что файл быз изменен
    */
    void modelDataChanged();

    /// @brief Слот-обработчик печати открытого файла.
    void on_actionPrint_triggered();

    /// @brief Слот-обработчик открытия окна  с графиком.
    void on_actionGrapf_triggered();


    /**
    * @brief Обработчик события переключения языка.
    *
    * Этот метод меняет язык в соответсвии с выбранным
    * выполняет определенные действия, такие как удаление переводчика и установка других флажков.
    * @param Выбранная кнопка
    */

    void switch_Language(QAction *action);

    /**
     * @brief Слот-обработчик настройки toolBar.
     * @param Настройка отображения ToolBar.
    */
    void on_actionToolBar_toggled(bool arg1);

private:

    /**
    * @brief Обработчик события переключения языка.
    *
    * Этот метод меняет язык в соответсвии с выбранным
    * выполняет определенные действия, такие как удаление переводчика и установка других флажков.
    * @param язык из QLocale
    */
    void switchLanguage (QVariant language);
    /**
      * @brief Метод сохранения файла.
      * @param filePath Путь, куда нужно сохранить файл.
      * @return bool успешно ли сохранен файл
     */
    bool savefile(QString filePath);

    /**
      * @brief Метод сохранения файла.
      * @param filePath Путь, куда нужно откуда нужно загрузить файл.
      * @return bool успешно ли открыт файл
     */
    bool loadfile(QString filePath);

    /**
     * @brief Метод установки интерфейса в положение, открыт или нет файл
     */
    void setStateWhenFileOpened(bool);

    /**
     * @brief Метод предлагающий сохранить файл
     * @return bool, где false - пользователь отменет любое действие следующие за предложением сохранить файл
     */
    bool suggestSaveFile();


    /**
     * @brief Метод создающий регулярное выражение для фильтрации
     * @param data строка, основе которой нужно создать выражение для фильтрации
     * @return Регулярное выражение для фильтрации
     */
    QRegularExpression handleSortData(const QString &data);

    /**
     * @brief Метод, загружающий настройки
     */
    void loadSettings();

    /**
     * @brief Метод, сохраняющий настройки
     */
    void saveSettings();

    /**
     * @brief Метод, ставящий горизонтальным заголовкам стандартную(заготовленную) ширину
     */
    void setDefaultHeadersSizes();

private:
    /// @brief Cсылка на интерфейс класса
    Ui::MainWindow *ui;

    /// @brief Ссылка на хранение модель для tableview
    MyTableModel *tableModel;

    /// @brief Ссылка на модель для фильтрации и сортировки
    QSortFilterProxyModel *sortmodel;

    /// @brief Ссылка на окно для поиска
    FindDialog *findDialog;

    /// @brief Ссылка на окно для графика
    GrapfDialog *grapfDialog;

    /// @brief Строка, хранящая путь к текущему открытому файлу
    QString openedPath = "";

    /// @brief Bool, отмечающий, были ли изменения в файле
    bool somethingMightChanged = false;

    /// @brief Bool, говорящий, открыто ли окно графика
    bool grapfActive = false;
    /// @brief Создание переводчика
    QTranslator translator_Ru;
    QTranslator translator_Fr;
    /// @brief Создание переключателя языка
    QActionGroup *menuLanguage;

};
#endif // MAINWINDOW_H
