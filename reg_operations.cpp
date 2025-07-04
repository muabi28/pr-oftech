#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customtable.hpp"

#include <QMessageBox>
#include <QClipboard>

void MainWindow::on_actionAdd_row_triggered()
{
    int row = tableModel->rowCount(); // получаем кол-во строк

    // Создаем и вставляем пустой элемент в таблицу
    DataElement emptyElement;
    tableModel->appendRow(emptyElement);

    ui->tableView->setCurrentIndex(sortmodel->mapFromSource(tableModel->index(row, 0))); // Выделяем нововставленную строку
    modelDataChanged();
}


void MainWindow::on_actionRemove_row_triggered()
{
    // Получаем выделенный индекс, предварительно соотнеся его с индексом в исходной модели, тк может быть включена сортировка или фильтрация
    int currentRow = sortmodel->mapToSource(sortmodel->index(ui->tableView->currentIndex().row(), 0)).row();
    if (!tableModel->removeRow(currentRow)) // Удалем, и если это не успешно
    {
        // То говорим пользователю что таблица пуста
        QMessageBox::information(this, tr("Deletion Error"), tr("The table is empty"));
        return;
    }
}

void MainWindow::on_actionCopy_row_triggered()
{
    // Получаем выделенный индекс, предварительно соотнеся его с индексом в исходной модели, тк может быть включена сортировка или фильтрация
    int currentRow = sortmodel->mapToSource(sortmodel->index(ui->tableView->currentIndex().row(), 0)).row();
    if(currentRow != -1) // Если ряд выделен
    {
        // Копирует в буфер обмена Системы выделенную строку, предварительно переведенную в строку
        QApplication::clipboard()->setText(tableModel->getDataFromTable(currentRow).getSplitString());
    }
}

void MainWindow::on_actionCut_row_triggered()
{
    // Получаем выделенный индекс, предварительно соотнеся его с индексом в исходной модели, тк может быть включена сортировка или фильтрация
    int currentRow = sortmodel->mapToSource(sortmodel->index(ui->tableView->currentIndex().row(), 0)).row();
    if(currentRow != -1) // Если ряд выделен
    {
        // Копирует в буфер обмена Системы выделенную строку, предварительно переведенную в строку
        QApplication::clipboard()->setText(tableModel->getDataFromTable(currentRow).getSplitString());
        tableModel->overWriteData(currentRow); // И делает ряд пустым
    }
}

void MainWindow::on_actionPaste_row_triggered()
{
    // Получаем выделенный индекс, предварительно соотнеся его с индексом в исходной модели, тк может быть включена сортировка или фильтрация
    int currentRow = sortmodel->mapToSource(sortmodel->index(ui->tableView->currentIndex().row(), 0)).row();
    if(currentRow != -1) // Если ряд выделен
    {


        QString line = QApplication::clipboard()->text(); // Получаем строку для вставки
        if(line.count(";") != 8) // Проверяем, то что строка корректна и содержит 8 столбцов
        {
            QMessageBox msgBox(QMessageBox::Warning,
                               tr("Unsupported Input Data Format"),
                               tr("Data format is not supported.\nYou can paste only rows."),
                               QMessageBox::Ok);
            msgBox.exec(); return;// Если нет - сообщаем пользователю и ничего не перезаписываем
        }
        tableModel->overWriteData(currentRow, DataElement(line)); // Перезаписываем данные в выделенную строку
    }
}

