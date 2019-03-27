#include "createmessage.h"

QMessageBox* createMessage(const QString &sender, qint32 type, QVariant addInfo)
{
    QMessageBox* msg = new QMessageBox;
    if (type == 1)
    {
        msg->setText("Ошибка при обращении к базе данных.");
        if (sender == "tableMenu")
            msg->setText(msg->text() + "\nВозврат к главному меню.");
        msg->setIcon(QMessageBox::Critical);
    }
    if (sender == "routeForm")
    {
        if (type == 2)
        {
            msg->setText("К маршруту с номером " + addInfo.toString() + " приписан(-ы) автобус(-ы). \nУдаление маршрута невозможно.");
            msg->setIcon(QMessageBox::Warning);
        }
        if (type == 3)
        {
            msg->setText("Маршрут с номером " + addInfo.toString() + " уже существует.");
            msg->setIcon(QMessageBox::Warning);
        }
    }
    if (sender == "busForm")
    {
        if (type == 2)
        {
            msg->setText("К автобусу с номером " + addInfo.toString() + " приписан(-ы) водитель(-и). \nУдаление автобуса невозможно.");
            msg->setIcon(QMessageBox::Warning);
        }
    }
    if (sender == "controlForm")
    {
        if (type == 2)
        {
            msg->setText("Данный водитель уже работает на автобусе с ИД " + addInfo.toString());
            msg->setIcon(QMessageBox::Warning);
        }
    }
    if (sender == "otherForm")
    {
        if (type == 2)
        {
            msg->setText("К данному маршруту не приписано ни одного автобуса.");
            msg->setIcon(QMessageBox::Information);
        }
    }
    if (sender == "salaryForm" || sender == "typeForm")
    {
        if (type == 2)
        {
            msg->setText("Данные используются. Удаление невозможно.");
            msg->setIcon(QMessageBox::Warning);
        }
        if (type == 3)
        {
            msg->setText("Данные уже содержатся в базе");
            msg->setIcon(QMessageBox::Warning);
        }
    }
    if (sender == "mainMenu")
    {
        if (type == 2)
        {
            msg->setText("Неверный пароль");
            msg->setIcon(QMessageBox::Warning);
        }
        if (type == 3)
        {
            msg->setText("База данных не создана\nВыход из программы");
            msg->setIcon(QMessageBox::Critical);
        }
    }
    if (sender == "tableMenu")
    {
        if (type == 2)
        {
            msg->setText("Не удалось создать файл");
            msg->setIcon(QMessageBox::Warning);
        }
        if (type == 3)
        {
            msg->setText("Отчет создан успешно");
            msg->setIcon(QMessageBox::NoIcon);
        }
    }
    msg->exec();
    return msg;
}
