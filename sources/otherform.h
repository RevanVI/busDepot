#ifndef OTHERFORM_H
#define OTHERFORM_H

#include <QWidget>
#include <QtSql>
#include "createmessage.h"

namespace Ui {
class otherForm;
}

class otherForm : public QWidget
{
    Q_OBJECT

public:
    explicit otherForm(qint32 tabN, QWidget *parent = nullptr);
    ~otherForm();

private:
    Ui::otherForm *ui;
    qint32 tabNum;
    void getList();

private slots:
    void createQuery();
    void cancelQuery();
signals:
    void setQuery(QSqlQuery*);
    void defaultQuery();
};

#endif // OTHERFORM_H
