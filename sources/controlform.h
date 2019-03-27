#ifndef CONTROLFORM_H
#define CONTROLFORM_H

#include <QWidget>
#include <QtSql>
#include "createmessage.h"

namespace Ui {
class controlForm;
}

class controlForm : public QWidget
{
    Q_OBJECT

public:
    explicit controlForm(const QString &Mode, QWidget *parent = nullptr);
    ~controlForm();

private:
    Ui::controlForm *ui;
    QString mode;
    bool getControlList();

private slots:
    void addControl();
    void setDefaultInfo();
    void removeControl();
    void getDrivingBus();
signals:
    void dataChanged();
};

#endif // CONTROLFORM_H
