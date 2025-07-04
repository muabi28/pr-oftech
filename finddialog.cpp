#include "finddialog.h"
#include "ui_finddialog.h"

#include <QCloseEvent>

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    orginalTitle = this->windowTitle();
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::closeEvent(QCloseEvent *event)
{
    ui->lineEdit->setText(""); // Устанавливаем пустую строку в lineEdit для ввода
    emit this->findclosed(); // Издаем сигнал о том что окно поиска закрыто
    event->accept(); // Принимаем событие об зактытии окна
}

void FindDialog::on_closeButton_clicked()
{
    this->close(); // Закрываем файл
}


void FindDialog::setNewWindowName(QString parentPart)
{
    // Формируем новое название окна
    QString windowname = orginalTitle + tr(" in ");
    windowname += parentPart;
    setWindowTitle(windowname);

}


void FindDialog::on_lineEdit_textChanged(const QString &arg1)
{
    emit this->findSignal(ui->columnBox->currentIndex() - 1, arg1); // издаем сигнал о том что нужно осущесвить фильтрацию (поиск)
}

void FindDialog::on_columnBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    emit this->findSignal(ui->columnBox->currentIndex() - 1, ui->lineEdit->text()); // издаем сигнал о том что нужно осущесвить фильтрацию (поиск)
}



