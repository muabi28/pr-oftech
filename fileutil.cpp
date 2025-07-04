#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QMessageBox>
#include <QFile>


bool MainWindow::savefile(QString filePath)
{
    QFile file(filePath); // Создаем объект для работы с файлом
    if (!file.open(QIODevice::WriteOnly)) // Если файл не открывается для записи то ...
    {
        QMessageBox::warning(this, tr("File saving error"), tr("File not saved")); return false; // ... Сообщаем пользоватею об этом и ничего не сохраняем
    }

    int rows = tableModel->rowCount(); // Получаем кол-во строк
    QTextStream fileout(&file); // Создаем поток для записи в файл
    fileout << "~!~This~string~checks~file~can~be~opened~!~\n"; // Вписываем особую строку для проверки поддерживаемости файла
    for (int row = 0; row < rows; ++row)
    {
        DataElement tempelem = tableModel->getDataFromTable(row); // Получаем данные строки
        fileout << tempelem.getSplitString() + "\n"; // Записываем файл построчно, получая тектовую версию строки разделенную точку с запятой между элементамии
    }
    file.close(); // Закрываем файл
    somethingMightChanged = false; // Выключаем флаг о том, что есть несохраенные изменения (тк мы все сохранили)
    return true;
}

bool MainWindow::loadfile(QString filePath)
{
    if (filePath.isEmpty()) // Если выбранный путь файла для открытия пустой ...
    {
        QMessageBox::warning(this, tr("File opening error"), tr("File not opened")); // ... То сообщаем пользователю и нечего не открываем
        return false;
    }

    QFile file(filePath); // Создаем объект для работы с файлом

    if (!file.open(QFile::ReadOnly | QFile::Text)) // Если файл не открывается для чтения то ..
    {
        QMessageBox::warning(this, tr("File opening error"), tr("File not opened")); return false;// ... Сообщаем пользоватею об этом и ничего не записываем
    }

    QTextStream filein(&file); // Создаем поток для чтения из файла

    // Если первая строка в файле - особая строка программы,то продолжаем, иначе говорим что файл не поддерживается ...
    if(filein.readLine() != "~!~This~string~checks~file~can~be~opened~!~")
    {
        QMessageBox::warning(this, tr("File opening error"), tr("File not supported")); return false;
        file.close(); // ... И закрываем файл
        return false;
    }

    tableModel->dropTable(); // Очищаем таблицу

    while (!filein.atEnd())  // Пока не дойдем до конца файла
    {
        QString data = filein.readLine(); // Получаем новую строку
        tableModel->appendRow(DataElement(data)); // Добавляем в список таблицы новую строку
    }
    file.close(); // Закрываем строку
    return true;
}

bool MainWindow::suggestSaveFile()
{
    if(openedPath == "" or !somethingMightChanged) return true; // Если ничего не менялось, файл не открыт то смысла предлагать сохранять нет

    // Уточняем у пользователя, нужно ли сохранять файл ..
    QMessageBox::StandardButton reply =
        QMessageBox::question(this,
                              tr("File might have changes"),
                              tr("Do you want to save it?"),
                              QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

    if(reply==QMessageBox::Save) // ... Если да ...
    {
        if(openedPath == "~untitled.db")
            ui->actionSave_as->trigger(); // ... То если файл новый предлагаем сохранить как ...
        else ui->actionSave->trigger(); // ... Иначе сохраняем по существующему пути
        return true;
    }
    if(reply==QMessageBox::No) // ... а если нет ...
    {
        return true; // ... То ничего не делаем
    }
    else return false;
}


