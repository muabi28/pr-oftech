#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customtable.hpp"

#include "finddialog.h"

#include <QMessageBox>
#include <QActionGroup>
#include <QLocale>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QSettings>
#include <QPrinter>
#include <QPrintDialog>
#include <QDir>
#include <QFileInfo>
#include <QModelIndex>
#include <QPainter>
#include <QFontMetrics>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) // Наследуемся от QMainWindow и инициализируем его
    , ui(new Ui::MainWindow) // Создаем экземпряр интрерфейса для окна
    , tableModel(new MyTableModel(this)) // Создаем модель для таблицы
    , sortmodel(new QSortFilterProxyModel(this)) // Создаем модель для сортировки и поиска(фильтрации) в таблице
    ,menuLanguage (new QActionGroup (this))
{
    ui->setupUi(this); // Инициализируем интерфейс

    loadSettings(); // Подгружаем сохраненные настройки

    menuLanguage->addAction(ui->actionEnglish);
    menuLanguage->addAction(ui->actionRussian);
    menuLanguage->addAction(ui->actionFrench);

    ui->actionEnglish->setData(QLocale::English);
    ui->actionRussian->setData(QLocale::Russian);
    ui->actionFrench->setData(QLocale::French);


    connect(menuLanguage, &QActionGroup::triggered, this, &MainWindow::switch_Language);


    if (!translator_Ru.load("tr/pr-oftech_ru_RU.qm")) //загружаем русский перевод
        QMessageBox::warning(this, "pr-oftech", tr ("Russian translation not loaded"));
    if (!translator_Fr.load("tr/pr-oftech_fr_FR.qm")) //загружаем русский перевод
        QMessageBox::warning(this, "pr-oftech", tr ("French translation not loaded"));
    setStateWhenFileOpened(false); // Ставим состояние, когда файл закрыт (выключаем некоторые действия)

    sortmodel->setSourceModel(tableModel); // Добавляем основную модель в модель сортировки/поиска как исходную
    ui->tableView->setModel(sortmodel); // Устанавливаем модель сортировки как модель данных для таблицы
    sortmodel->setFilterCaseSensitivity(Qt::CaseInsensitive); // Ставим фильтр, чтобы поиск не зависел от регистра
    connect(tableModel, SIGNAL(modelDataChanged()), this, SLOT(modelDataChanged())); // Подключаем слот, который реагирует на изменение данных в модели
    ui->tableView->verticalHeader()->hide(); // Прячем вертикальные заголовки

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu); // Включаем политку получения контекстного меню по нажатию правой кнопки мыши
    connect(ui->tableView, &QWidget::customContextMenuRequested, this, &MainWindow::showCustomContextMenu); // Подкючаем сигнал запроса контекстного меню к слоту показа этого меню

    grapfDialog = new GrapfDialog(); // Создаем окно для отображения графиков
    findDialog = new FindDialog(this); // Создаем окно для осуществления поиска
    connect(findDialog, SIGNAL(findSignal(int, const QString&)), this, SLOT(findHandler(int, const QString&))); // Подключаем обработку поиска к сигналу о том что нужно сделать поиск в таблице
    connect(findDialog, SIGNAL(findclosed()), this, SLOT(findCloseHandler())); // Подключаем обработчик закрытия окна с поиском к сигналу закрытия

    setDefaultHeadersSizes(); // Устанавливаем заранее заготовленне размеры для заголовков столбцов
    originalTitle = this->windowTitle(); // Сохраняем первоначальное название окна для дальнейшего формирования динамического названия на основе работы с файлом
    this->activateWindow(); // Ставим окно выше всех существующих окон (Нужно чтобы вновь созданное окно не было за вызывающим окном, а сразу показывалось пользователю)
}

void MainWindow::setDefaultHeadersSizes()
{
    ui->tableView->horizontalHeader()->resizeSection(0, 115); // Устанавливаем размер для столбца "Тип устройства"
    ui->tableView->horizontalHeader()->resizeSection(1, 85);  // Устанавливаем размер для столбца "Категория"
    ui->tableView->horizontalHeader()->resizeSection(2, 110); // Устанавливаем размер для столбца "Производитель"
    ui->tableView->horizontalHeader()->resizeSection(3, 110); // Устанавливаем размер для столбца "Модель"
    ui->tableView->horizontalHeader()->resizeSection(4, 125); // Устанавливаем размер для столбца "Дата поступления"
    ui->tableView->horizontalHeader()->resizeSection(5, 120); // Устанавливаем размер для столбца "Цена"
    ui->tableView->horizontalHeader()->resizeSection(6, 165); // Устанавливаем размер для столбца "Местонахождение"
    ui->tableView->horizontalHeader()->resizeSection(7, 145); // Устанавливаем размер для столбца "Срок эксплуатации"
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!suggestSaveFile()) // Предлагаем сохранить открытый файл и если пользователь нажал кнопку "Отменить"...
    {
        event->ignore(); return; // ... то игнорируем событие выхода и не закрываем окно
    }

    QMessageBox::StandardButton reply =
        QMessageBox::question(this,
                              tr("Exit from app"),
                              tr("Close app?"),
                              QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) // Если нажата кнопка "Да" - то готовимся к закрытию окна
    {
         saveSettings(); // Сохраняем настройки
        if(findDialog) findDialog->close(); // Если есть открытый диалог поиска - закрываем его
        if(grapfDialog) grapfDialog->close(); // Если есть открытый диалог с графиком - закрываем его

        event->accept(); // Принимаем событие закрытия окна и окно закрывается
    }
    else // Иначе считаем что пользователь хочет остаться в приложении ...
    {
        event->ignore(); // ... И игнорируем событие закрытиия окна и тем самым не закрываем окно
    }
}

MainWindow::~MainWindow()
{
    // Удаляем объекты и тем самым освобождаем память


    delete tableModel;
    delete sortmodel;
    delete findDialog;
    delete grapfDialog;
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("MGSU", "pr"); // Создаем объет для работы с настройками. Укаываем название организации и название приложения

    settings.beginGroup("MainWindowGeometry"); // Начало группы настроек связанной с геометрией окна
    resize(settings.value("size", QSize(400, 400)).toSize()); // Достаем сохраненный размер окна из настроек, если такой настройки нет, используем значения по умолчанию
    move(settings.value("position", QPoint(200, 200)).toPoint()); // Достаем сохраненное положение окна из настроек, если такой настройки нет, используем значения по умолчанию
    settings.endGroup(); // Конец группы настроек связанной с геометрией окна


    settings.beginGroup("menuSettings"); // начало группы настроек связанной с настройками
    ui->actionToolBar->setChecked(settings.value("toolBar").toBool());
    on_actionToolBar_toggled(ui->actionToolBar->isChecked());
    switchLanguage(settings.value("RU"));
    settings.endGroup(); // Конец группы настроек связанной с геометрией окна
}

void MainWindow::saveSettings()
{
    QSettings settings("MGSU", "pr");// Создаем объет для работы с настройками. Укаываем название организации и название приложения"
    qDebug() << "test";


    settings.beginGroup("MainWindowGeometry"); // начало группы настроек связанной с геометрией окна
    settings.setValue("size", size()); // Записываем размер окна на основе текущего размера окна
    settings.setValue("position", pos()); // Записываем размер окна на основе текущего положения окна
    settings.endGroup(); // Конец группы настроек связанной с геометрией окна

    settings.beginGroup("menuSettings"); // начало группы настроек связанной с настройками
    settings.setValue ("toolBar", ui->actionToolBar->isChecked()); // Сохраняем состояние toolBar
    settings.setValue("RU",menuLanguage->checkedAction()->data()); // Сохраняем состояние язык
    settings.endGroup(); // Конец группы настроек связанной с геометрией окна




}

void MainWindow::on_actionNew_triggered()
{
    if(!suggestSaveFile()) // Предлагаем сохранить открытый файл и если пользователь нажал кнопку "Отменить"...
        return; // ... то игнорируем создания файла и выходим из функции
    if(findDialog) findDialog->close(); // Если открыто окно с поиском - закрываем его
    if(grapfDialog) grapfDialog->close(); // Если открыто окно с графиком - закрываем его
    tableModel->dropTable(); // Удаляем все данные из контейнера данных модели
    openedPath = "~untitled.db"; // Ставим название файла как неназванное
    somethingMightChanged = true; // Включаем флаг о том, что есть несохраенные изменения (тк файл новый, фактически он не сохранен)
    setStateWhenFileOpened(true); // Ставим состояние, когда файл открыт (включаем некоторые действия)
    setDefaultHeadersSizes(); // Устанавливаем заранее заготовленне размеры для заголовков столбцов
    setNewWindowTitle(); // Вызываем установщик нового имени окна на основе открытого файла (оно установится и вычислится внутри метода)
    tableModel->appendRow(); // Добавляем один пустой ряд, чтобы таблица не была пустой
    ui->tableView->selectRow(0); // Выделяем этот ряд
}

void MainWindow::on_actionOpen_triggered()
{
    if(!suggestSaveFile()) // Предлагаем сохранить открытый файл и если пользователь нажал кнопку "Отменить"...
        return; // ... то игнорируем открытие файла и выходим из функции

    // Используем стандартный диалог открытия файла для получения пути к файлу, который хочет открыть пользователь
    QString filePath =
        QFileDialog::getOpenFileName(this,
                                     tr("Open File"),
                                     QDir::currentPath(),
                                     tr("Text Files( *.txt , *.dat , *.db);;All Files (*)"),
                                     nullptr); // Используем окно операционной системы
    if(loadfile(filePath)) // Загружаем файл в модель и если это прошло успешно то:
    {
        if(findDialog) findDialog->close(); // Если открыто окно с поиском - закрываем его
        if(grapfDialog) grapfDialog->close(); // Если открыто окно с графиком - закрываем его
        openedPath = filePath; // Сохраняем путь к открытому файлу
        setStateWhenFileOpened(true); // Ставим состояние, когда файл открыт (включаем некоторые действия)
        somethingMightChanged = false; // Выключаем флаг о том, что есть несохраенные изменения
        ui->tableView->setCurrentIndex(sortmodel->sourceModel()->index(0,0)); // Выделяем индекс 0, 0 - чтобы сразу можно было шорткатами копировать или вырезать строку
        ui->tableView->selectRow(0); // Выделяем первый ряд, там где установлен индекс
        ui->tableView->resizeColumnsToContents(); // Подгоняем ширину колонок под содержимое
        setNewWindowTitle(); // Вызываем установщик нового имени окна на основе открытого файла (оно установится и вычислится внутри метода)
    }
}


void MainWindow::on_actionClose_triggered()
{
    if(!suggestSaveFile()) // Предлагаем сохранить открытый файл и если пользователь нажал кнопку "Отменить"...
        return; // ... то игнорируем открытие файла и выходим из функции

    openedPath = ""; // Обнуляем путь к открытому файлу
    tableModel->dropTable(); // Удаляем все данные из контейнера данных модели
    if(findDialog) findDialog->close(); // Если открыто окно с поиском - закрываем его
    if(grapfDialog) grapfDialog->close(); // Если открыто окно с графиком - закрываем его
    somethingMightChanged = false; // Выключаем флаг о том, что есть несохраенные изменения (тк не существующий файл нельзя редоктровать и сохранять)
    setStateWhenFileOpened(false); // Ставим состояние, когда файл закрыт (выключаем некоторые действия)
    setNewWindowTitle(); // Вызываем установщик нового имени окна на основе открытого файла (оно установится и вычислится внутри метода)
}

void MainWindow::on_actionExit_triggered()
{
    this->close(); // Закрываем окно
}

void MainWindow::on_actionSave_triggered()
{
    if(openedPath == "~untitled.db") // Если наш файл новый (не сохранен и не имеет пути)
    {
        ui->actionSave_as->trigger(); // То издаем сигнал для вызова метода Сохранить Как...
        return;
    }

    // Иначе сохраняем файл по установленному пути
    savefile(openedPath); // Сохраняем файл
    setNewWindowTitle(); // Вызываем установщик нового имени окна на основе открытого файла (оно установится и вычислится внутри метода)
}

void MainWindow::on_actionSave_as_triggered()
{
    // Используем стандартный диалог сохранения файла для получения пути, куда пользователь хочет сохранить данные
    QString filePath =
        QFileDialog::getSaveFileName(this,
                                     tr("Save File"),
                                     QDir::currentPath(),
                                     tr("Text Files( *.txt , *.dat , *.db);;All Files (*)"),
                                     nullptr,
                                     QFileDialog::DontUseNativeDialog); // Используем специальное окно выбора пути сохранения, созданное QT

    if(savefile(filePath)) // Сохраняем файл и если это прошло успешно то:
    {
        if(openedPath == "~untitled.db") // Если мы сохранили НОВЫЙ файл, то ...
            openedPath = filePath; // Теперь мы работаем с существующим файлом.
        setNewWindowTitle(); // Вызываем установщик нового имени окна на основе открытого файла (оно установится и вычислится внутри метода)
    }
}

void MainWindow::setStateWhenFileOpened(bool state)
{
    ui->tableView->setEnabled(state); // Вкл-выкл возможность взаимодействия с таблицой
    ui->actionSave_as->setEnabled(state); // Вкл-выкл возможность "Сохранять как"
    ui->actionClose->setEnabled(state); // Вкл-выкл возможность закрывать файл
    ui->menuEdit->setEnabled(state); // Вкл-выкл возможность взаимодействия в пунктом "Редактировать" в меню
    ui->actionAdd_row->setEnabled(state); // Вкл-выкл возможность добавлять строку в талицу
    ui->actionRemove_row->setEnabled(state); // Вкл-выкл возможность удалять строку из талицы
    ui->actionFind->setEnabled(state); // Вкл-выкл возможность вызывать окно поиска
    ui->actionSortDeny->setEnabled(state); // Вкл-выкл возможность возвращать таблицу в исходное состаяние
    ui->menuFilter->setEnabled(state); // Вкл-выкл возможность взаимодействия в пунктом "Фильтр" в меню
    ui->actionPrint->setEnabled(state); // Вкл-выкл возможность выводить таблицу на печать
    ui->actionSave->setEnabled(state); // Вкл-выкл возможность выводить сохранять данные
    ui->actionGrapf->setEnabled(state); // Вкл-выкл возможность показывать график
}

void MainWindow::showCustomContextMenu(const QPoint& pos)
{
    QMenu menu; // Создаем объект контекстного меню

    menu.addAction(ui->actionRemove_row); // Добавляем в меню действие удаления строки
    menu.addAction(ui->actionCut_row); // Добавляем в меню действие вырезания строки
    menu.addAction(ui->actionCopy_row); // Добавляем в меню действие копирования строки
    menu.addAction(ui->actionPaste_row);

    if(sortmodel->sortColumn() != -1) // Если сортировка сейчас включена ...
    {
        menu.addSeparator(); // ... то добавляем разделитель
        menu.addAction(ui->actionSortDeny); // И добавляем в меню действие отмены сортировки
    }

    menu.exec(ui->tableView->viewport()->mapToGlobal(pos)); // Отображаем контекстное меню в местсе нахождения курсора
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::about(this,tr("About author"), tr("Ayubov Muabiya Gadzhievich ICTMS 2-3") ); // Показываем окно об авторе
}




void MainWindow::on_actionSortDeny_triggered()
{
    sortmodel->sort(-1); // Устанавливаем столбец сортировки -1, тем самым сбрасываем текущую сортировку в таблице
}

void MainWindow::on_actionNew_window_triggered()
{
    MainWindow *w = new MainWindow; // Создаем в динамической памяти новый экзммпляр MainWindow
    w->show(); // Показываем новое окно (оно не зависит от окна, которое его создает)
}

void MainWindow::setNewWindowTitle()
{
    QString windowname; // В строке windowname будем собирать будущее название нашего окна. Сначала добавим в нее имя файла, которое получаем из информации
    QFileInfo fileInfo(openedPath); // Получим объект с информацией по файлу, находящемуся у нас по открытому пути
    windowname += fileInfo.fileName(); // Сначала добавим в нее имя файла, которое получаем из информации о файле
    if(somethingMightChanged) windowname += "*"; // Если в файле были изменения, добавим звездочку в после имени файла
    if(findDialog) findDialog->setNewWindowName(windowname);

    if(grapfDialog) grapfDialog->setNewWindowName(windowname); // Если открыто окно графика - то передаем ему имя файла для дальнейшего формирования имени окна графика
    if(openedPath != "") windowname += " - "; // только если файл открыт, добавляем дефиз для создания формата имени {имяфайла} - {имяокна}.
    windowname += originalTitle; // И наконец добавляем оригинальное название окна, которое у него стоит в состоянии без открытого файла
    setWindowTitle(windowname); // Устанавливаем новое имя окна
}

void MainWindow::modelDataChanged()
{
    somethingMightChanged = true; // Тк мы получили сигнал об изменениях, включаем флаг, который это обозначает
    setNewWindowTitle(); // Обновляем имя окна, чтобы появилась звездочка после имени файла
    if(grapfDialog->isVisible()) //Если окно с графиком открыто то...
    {
        emit ui->actionGrapf->trigger(); // Делаем сигнал вызова графика, чтобы он перезаполнил данные
        grapfDialog->update(); // Переотрисовываем окно с учетом новой информации для обновления графика
    }
}

void MainWindow::on_actionPrint_triggered()
{

    if(!suggestSaveFile()) // Предлагаем сохранить открытый файл и если пользователь нажал кнопку "Отменить"...
        return; // ... то игнорируем открытие файла и выходим из функции

    QPrinter printer; // Создаем объект принтера
    QPrintDialog printdialog(&printer, this); // Cоздаем диалог для настройки нашего принтера, который в него подключаем
    printer.setPageOrientation(QPageLayout::Landscape); // Ставим настройку по умолчанию на Альбомную ориентацию, тк на книжной врядли поместится информация
    if (printdialog.exec() == QDialog::Rejected) return; // Если пользователь вышел из диалога печати, то выходим из метода и ничего не печатаем
    int totalRows = tableModel->rowCount(); // Получаем кол-во строк в исходной таблице через ее модель

    QTableView tempTableView; // Создаем временный (воображаемый) объект таблицы

    int paperheight = printer.pageLayout().paintRectPixels(printer.resolution()).size().height(); // Получаем высоту выбранной бумаги для печати
    int paperwidth = printer.pageLayout().paintRectPixels(printer.resolution()).size().width(); // Получаем ширину выбранной бумаги для печати

    tempTableView.resize(printer.pageLayout().paintRectPixels(printer.resolution()).size()); // Подгоняем размер временной таблицы под размер листа
    tempTableView.verticalHeader()->hide(); // Прячем вертикальные заголовки (индексы), чтобы они не печатались
    tempTableView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Выключаем вертикальный полузунок, чтобы он не печатался
    tempTableView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Выключаем горизонтальный ползунок, чтобы он не печатался
    tempTableView.setStyleSheet("QTableView { border: none; }"); // Отключаем внешнюю границу у таблицы, иначе на листе будут некрасивые рамки

    MyTableModel tempModel; // Создаем временную модель данных
    tempTableView.setModel(&tempModel); // И устанавливаем эту модель в таблицу

    QPainter painter(&printer); // Создаем объект QPainter, который будет рисовать на принтере

    // Устанавливаем относительные размеры для столбцов на основе полученной ширины страницы для печати
    tempTableView.horizontalHeader()->resizeSection(0, paperwidth * 0.27);
    tempTableView.horizontalHeader()->resizeSection(1, paperwidth * 0.03);
    tempTableView.horizontalHeader()->resizeSection(2, paperwidth * 0.05);
    tempTableView.horizontalHeader()->resizeSection(3, paperwidth * 0.15);
    tempTableView.horizontalHeader()->resizeSection(4, paperwidth * 0.10);
    tempTableView.horizontalHeader()->resizeSection(5, paperwidth * 0.10);
    tempTableView.horizontalHeader()->resizeSection(6, paperwidth * 0.12);
    tempTableView.horizontalHeader()->resizeSection(7, paperwidth * 0.18);


    QFontMetrics fontMetrics(QApplication::font()); // Получаем объет с информацией о шрифте в приложении

    // Пускаем цикл, обрабатывающий создание страницы
    // Цикл закончится, когда все строки будут напечатаны
    for(int rowsAlreadyPrinted = 0; rowsAlreadyPrinted < totalRows ;)
    {
        // Теперь запускаем цикл по строкам на листе
        // Цикл завершается, когда на наем закончится высота для печати
        // ЛИБО цикл завершается, если все строки изначальной таблицы распечатаны
        for(int rowInPage = 0, paperheigntUsed = 0;
             paperheigntUsed < paperheight && rowsAlreadyPrinted < totalRows;
             ++rowInPage, rowsAlreadyPrinted++)
        {
            DataElement insertememet = tableModel->getDataFromTable(rowsAlreadyPrinted); // Берем элемент, который будем вставлять в таблицу для печати
            int nameCellinTableWidth = tempTableView.horizontalHeader()->sectionSize(0); // Определяем длину ячейки с названием актива
            int namePixelWidth = fontMetrics.horizontalAdvance(insertememet.getdeviceModel()); // Определяем длину названия актива
            double hyphenationCoff = (double)namePixelWidth/nameCellinTableWidth; // Получаем отношение длины названия ко вмещаемой длине
            int newRowHeight = hyphenationCoff < 1 ? 30 : ceil(hyphenationCoff) * 30; // Определяем новую высоту строки
            paperheigntUsed += newRowHeight; // Добавляем к использованной высоте то что мы сейчас будем вставлять
            if(paperheigntUsed > paperheight) break; // Если на странице больше нет места для размещения текущей строки - выходим из цикла и переходим на следующую страницу

            tempModel.appendRow(insertememet); //Добавляем нужную строку из исходной модели во временную
            tempTableView.setRowHeight(rowInPage, newRowHeight); //Устанавливаем нужную высоту строки
        }
        tempTableView.render(&painter); // Отрисовываем временную таблицу
        if(rowsAlreadyPrinted < totalRows) // Если еще остались строки, которые нужно печатать ...
            printer.newPage(); // ... То создаем для их новую страницу в принтере
        tempModel.dropTable(); // Очищаем таблицу, чтобы вставлять в нее новые строки для нового листа
    }

}

void MainWindow::on_actionGrapf_triggered()
{
    grapfDialog->dropMapData(); // Очищаем данные для построения графика
    int rows = tableModel->rowCount(); // Получаем кол-во строк в таблице
    bool emptyDataTable = true; // Определяем флаг, который говорит, есть ли комбинация (год;цена) где оюа параметра не равны нулю

    // Пускаем цикл по рядам таблицы
    for (int row = 0; row < rows; ++row)
    {
        DataElement tempelem = tableModel->getDataFromTable(row); // Получаем временный объект, содержащий данные строки
        QString manufacturer = tempelem.getmanufacturer(); // Получаем производителя
        double price = tempelem.getprice(); // Получаем цену

        if(manufacturer == "" || price <= 0) continue; // Если дата поступления или цена меньше или равны нулю, то переходим к следующей строке
        if(emptyDataTable) emptyDataTable = false; // Иначе, если таблица до сих пор была пуста - ставим флаг что теперь не пуста
        grapfDialog->insertIntoMap(manufacturer, price); // Передаем производителя и цену в словарь данных графика
    }
    if(emptyDataTable) return; // Если таблица после обработки пустая - то смысла делать по ней график нету
    grapfDialog->show(); // Показываем окно с графиком
}


void MainWindow::switchLanguage(QVariant lang)
{
    switch (lang.toInt()){
    case QLocale::English:
        qApp->removeTranslator(&translator_Ru);
        qApp->removeTranslator(&translator_Fr);
        break;
    case QLocale::Russian:
        qApp->removeTranslator(&translator_Fr);
        qApp->installTranslator(&translator_Ru);
        break;
    case QLocale::French:
        qApp->removeTranslator(&translator_Ru);
        qApp->installTranslator(&translator_Fr);
        break;
    }
    ui->retranslateUi(this);
}

void MainWindow::switch_Language(QAction *action)
{
    switch (action->data().toInt()){
    case QLocale::English:
        qApp->removeTranslator(&translator_Ru);
        qApp->removeTranslator(&translator_Fr);
        break;
    case QLocale::Russian:
        qApp->removeTranslator(&translator_Fr);
        qApp->installTranslator(&translator_Ru);
        break;
    case QLocale::French:
        qApp->removeTranslator(&translator_Ru);
        qApp->installTranslator(&translator_Fr);
        break;
    }
    ui->retranslateUi(this);
}


void MainWindow::on_actionToolBar_toggled(bool arg1)
{
    ui->toolBar->setVisible(arg1);
}

