#include "mainwindow.h"
#include "finddialog.h"

void MainWindow::on_actionFind_triggered()
{
    findDialog->show(); // Показываем диалоговое окно с интерфейсом для поиска
}

void MainWindow::findHandler(int index, const QString &data)
{
    sortmodel->setFilterKeyColumn(index); // Указываем целевой столбец для поиска
    sortmodel->setFilterRegularExpression(handleSortData(data)); // Устанавливаем регулярное выражение для фильтрации
}

void MainWindow::findCloseHandler()
{
    sortmodel->setDynamicSortFilter(false);  // Отключение сортировки в модели
    sortmodel->setFilterRole(Qt::DisplayRole); // Устанавливаем пустую строку как регулярное выражение чтобы сбросить результат фильтрации и вернуть таблицу и исходный вид
}

QRegularExpression MainWindow::handleSortData(const QString &data)
{
    QRegularExpression regExp(data, QRegularExpression::PatternOption::ExtendedPatternSyntaxOption);
    return regExp;
}

