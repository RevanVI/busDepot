#ifndef BUSFORM_H
#define BUSFORM_H

#include <QWidget>
#include <QtSql>
#include "createmessage.h"

namespace Ui {
class busForm;
}

class busForm : public QWidget
{
    Q_OBJECT

public:
    explicit busForm(const QString &Mode, QWidget *parent = nullptr);
    ~busForm();

private:
    Ui::busForm *ui;
    QString mode;
    bool getBusList();

private slots:
    void getBusInfo();
    void setBusInfo();
    void addBus();
    void setDefaultInfo();
    void removeBus();
    void setCapacity();
signals:
    void dataChanged();
};

#endif // BUSFORM_H
