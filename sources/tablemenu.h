#ifndef TABLEMENU_H
#define TABLEMENU_H

#include <QtWidgets>
#include <QtSql>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "routeform.h"
#include "driverform.h"
#include "busform.h"
#include "controlform.h"
#include "otherform.h"
#include "salaryform.h"
#include "typeform.h"

namespace Ui {
class TableMenu;
}

class TableMenu : public QWidget
{
    Q_OBJECT

public:
    explicit TableMenu(int r, QWidget *parent = nullptr);
    ~TableMenu();

signals:
    void backButtonClick();

private slots:
    void on_pushButton_pressed();
    void widgetCreate(); //создает виджеты для взаимодействия с БД
    void refresh(int tab = -1); //обновляет таблицы по сигналу от форм
    void tableChoice(int tabN);
    void setTabQuery(QSqlQuery* q = nullptr);
    void createReport();
private:
    Ui::TableMenu *ui;
    qint32 rang;
    QSqlQueryModel** tabModels;
    int prevTableNum;
    int tableNum;
    QSqlQuery* tabQuery;

    void initializeTabs(quint32 tabCount);
    void initializeWidgets(quint32 tabCount);
    void createQuery(qint32 tabN); //создает и присваивает вкладке tab запрос

};

#endif // TABLEMENU_H
