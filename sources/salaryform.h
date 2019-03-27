#ifndef SALARYFORM_H
#define SALARYFORM_H

#include <QWidget>
#include <QtSql>
#include "createmessage.h"

namespace Ui {
class salaryForm;
}

class salaryForm : public QWidget
{
    Q_OBJECT

public:
    explicit salaryForm(const QString &Mode, QWidget *parent = nullptr);
    ~salaryForm();

private:
    Ui::salaryForm *ui;
    QString mode;
private slots:
    void getClassList();
    void getExpList();
    void getSalaryInfo();
    void setSalaryInfo();
    void addSalary();
    void setDefaultInfo();
    void removeSalary();
signals:
    void dataChanged();
};

#endif // SALARYFORM_H
