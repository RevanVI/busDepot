#ifndef ROUTEFORM_H
#define ROUTEFORM_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include "createmessage.h"

namespace Ui {
class routeForm;
}

class routeForm : public QWidget
{
    Q_OBJECT

public:
    explicit routeForm(const QString &Mode, QWidget *parent = nullptr);
    ~routeForm();

private:
    Ui::routeForm *ui;
    QString mode;
    bool getRouteList();
    bool isUniqueIds();

private slots:
    void getRouteInfo();
    void setRouteInfo();
    void addRoute();
    void setDefaultInfo();
    void removeRoute();
signals:
    void dataChanged();
};

#endif // ROUTEFORM_H
