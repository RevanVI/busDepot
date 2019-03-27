#ifndef TYPEFORM_H
#define TYPEFORM_H

#include <QWidget>
#include <QtSql>
#include "createmessage.h"

namespace Ui {
class typeForm;
}

class typeForm : public QWidget
{
    Q_OBJECT

public:
    explicit typeForm(const QString& Mode, QWidget *parent = nullptr);
    ~typeForm();

private:
    Ui::typeForm *ui;
    QString mode;
private slots:
    void getTypeList();
    void getCapacityInfo();
    void setCapacityInfo();
    void addType();
    void setDefaultInfo();
    void removeType();
signals:
    void dataChanged();
};

#endif // TYPEFORM_H
