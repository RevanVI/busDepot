#ifndef DRIVERFORM_H
#define DRIVERFORM_H

#include <QWidget>
#include <QtSql>
#include "createmessage.h"

namespace Ui {
class driverForm;
}

class driverForm : public QWidget
{
    Q_OBJECT

public:
    explicit driverForm(const QString& Mode, QWidget *parent = nullptr);
    ~driverForm();

private:
    Ui::driverForm *ui;
    QString mode;

    //bool isUniqueIds();

private slots:
    void getDriverList();
    void getClassList();
    void getExpList();
    void getDriverInfo();
    void setDriverInfo();
    void addDriver();
    void setDefaultInfo();
    void removeDriver();
    void getSalary();
signals:
    void dataChanged();
};

#endif // DRIVERFORM_H
