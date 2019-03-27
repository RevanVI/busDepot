#include "tablemenu.h"
#include "ui_tablemenu.h"

TableMenu::TableMenu(int r, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableMenu)
{
    ui->setupUi(this);
    rang = r;
    tableNum = -1;
    tabQuery = nullptr;
    ui->label->setText("Пользователь: ");
    if (rang == 0)
        ui->label->setText(ui->label->text() + "администратор");
    else if (rang == 1)
        ui->label->setText(ui->label->text() + "диспетчер");
    else
        ui->label->setText(ui->label->text() + "водитель");
    QSqlDatabase db = QSqlDatabase::database();
    bool ok = db.open();
    if (!ok)
    {
        qDebug() << db.lastError().text();
        createMessage("tableMenu", 1);
        emit backButtonClick();
    }
    quint32 tabCount = 7;

    initializeTabs(tabCount);
    initializeWidgets(tabCount);
    QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refresh(int)));
    QObject::connect(ui->createReport, SIGNAL(clicked()), this, SLOT(createReport()));
}

TableMenu::~TableMenu()
{
    for (qint32 i = 0; i < ui->tabWidget->count(); ++i)
    {
        delete tabModels[i];
    }
    delete tabModels;
    delete ui;
    QSqlDatabase db = QSqlDatabase::database();
    db.close();
}

void TableMenu::on_pushButton_pressed()
{
    emit backButtonClick();
}

void TableMenu::initializeTabs(quint32 tabCount)
{
    tabModels = new QSqlQueryModel*[tabCount];
    for (quint32 i = 0; i < tabCount; ++i)
    {
        tabModels[i] = new QSqlQueryModel(ui->tabWidget);;
        QString tabName;
        QTableView* tabWidget = new QTableView();
        switch (i)
        {
        case 0:
            tabName = "Маршруты";
            break;
        case 1:
            tabName = "Водители";
            break;
        case 2:
            tabName = "Автобусы";
            break;
        case 3:
            tabName = "Водитель-автобусы";
            break;
        case 4:
            tabName = "Другие данные";
            break;
        case 5:
            tabName = "Зарплата";
            break;
        case 6:
            tabName = "Тип автобуса";
            break;
        }
        createQuery(i);
        tabWidget->setModel(tabModels[i]);
        ui->tabWidget->addTab(tabWidget, tabName);
    }
}

void TableMenu::initializeWidgets(quint32 tabCount)
{
    QWidget* wdgt;
    QVBoxLayout* layt;
    for (quint32 i = 0; i < tabCount; ++i)
    {
        wdgt = new QWidget(ui->stackedWidget);
        layt = new QVBoxLayout();
        if (i == 4)
        {
            QGroupBox* grBox = new QGroupBox("Таблицы", wdgt);
            QVBoxLayout* grLayt = new QVBoxLayout();
            QButtonGroup* tableCh = new QButtonGroup(wdgt);
            tableCh->setObjectName("tableCh");
            QRadioButton* rButton = new QRadioButton("Автобус-водители");
            tableCh->addButton(rButton, 0);
            grLayt->addWidget(rButton);
            rButton = new QRadioButton("Маршрут-автобусы");
            tableCh->addButton(rButton, 1);
            grLayt->addWidget(rButton);
            rButton = new QRadioButton("Маршрут-водители");
            tableCh->addButton(rButton, 2);
            grLayt->addWidget(rButton);
            grBox->setLayout(grLayt);
            QPushButton* choiceButton = new QPushButton("Выбрать", wdgt);
            choiceButton->setObjectName("otherChoice");
            layt->addWidget(grBox);
            layt->addWidget(choiceButton);
            QObject::connect(choiceButton, SIGNAL(clicked()), this, SLOT(widgetCreate()));
            QObject::connect(tableCh, SIGNAL(buttonClicked(int)), this, SLOT(tableChoice(int)));
            prevTableNum = 0;

        }
        else if ((rang == 0 || rang == 1) && i < 4)
        {
            QPushButton* addButton = new QPushButton("Добавить", wdgt);
            QPushButton* removeButton = new QPushButton("Удалить", wdgt);
            QPushButton* editButton;
            QObject::connect(removeButton, SIGNAL(clicked()), this, SLOT(widgetCreate()));
            QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(widgetCreate()));
            layt->addWidget(addButton);
            layt->addWidget(removeButton);
            if (i < 3)
            {
                editButton = new QPushButton("Изменить", wdgt);
                QObject::connect(editButton, SIGNAL(clicked()), this, SLOT(widgetCreate()));
                layt->addWidget(editButton);
            }
            if (i == 0) //Вкладка "маршруты"
            {
                addButton->setObjectName("routeAdd");
                removeButton->setObjectName("routeRemove");
                editButton->setObjectName("routeEdit");
            }
            else if (i == 1) //вкладка "водители"
            {
                addButton->setObjectName("driverAdd");
                removeButton->setObjectName("driverRemove");
                editButton->setObjectName("driverEdit");
            }
            else if (i == 2) //вкладка "автобусы"
            {
                addButton->setObjectName("busAdd");
                removeButton->setObjectName("busRemove");
                editButton->setObjectName("busEdit");
            }
            else if (i == 3) //вкладка "водитель-автобусы"
            {
                addButton->setObjectName("controlAdd");
                removeButton->setObjectName("controlRemove");
            }
        }
        else if (rang == 0 && i > 4)
        {
            QPushButton* addButton = new QPushButton("Добавить", wdgt);
            QPushButton* removeButton = new QPushButton("Удалить", wdgt);
            QPushButton* editButton = new QPushButton("Изменить", wdgt);
            QObject::connect(removeButton, SIGNAL(clicked()), this, SLOT(widgetCreate()));
            QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(widgetCreate()));
            QObject::connect(editButton, SIGNAL(clicked()), this, SLOT(widgetCreate()));
            layt->addWidget(addButton);
            layt->addWidget(removeButton);
            layt->addWidget(editButton);
            if (i == 5)
            {
                addButton->setObjectName("salaryAdd");
                removeButton->setObjectName("salaryRemove");
                editButton->setObjectName("salaryEdit");
            }
            else if (i == 6)
            {
                addButton->setObjectName("typeAdd");
                removeButton->setObjectName("typeRemove");
                editButton->setObjectName("typeEdit");
            }
        }
        wdgt->setLayout(layt);
        ui->stackedWidget->addWidget(wdgt);
    }
}

void TableMenu::widgetCreate()
{
    QString send = sender()->objectName();
    if (send.startsWith("route"))
    {
        routeForm* form;
        if (send == "routeAdd")
            form = new routeForm("add");
        if (send == "routeRemove")
            form = new routeForm("remove");
        if (send == "routeEdit")
            form = new routeForm("edit");
        form->show();
        form->setObjectName("routeForm");
        QObject::connect(form, SIGNAL(dataChanged()), this, SLOT(refresh()));
    }
    else if (send.startsWith("driver"))
    {
        driverForm* form;
        if (send == "driverAdd")
            form = new driverForm("add");
        if (send == "driverRemove")
            form = new driverForm("remove");
        if (send == "driverEdit")
            form = new driverForm("edit");
        form->show();
        form->setObjectName("driverForm");
        QObject::connect(form, SIGNAL(dataChanged()), this, SLOT(refresh()));
    }
    else if (send.startsWith("bus"))
    {
        busForm* form;
        if (send == "busAdd")
            form = new busForm("add");
        if (send == "busRemove")
            form = new busForm("remove");
        if (send == "busEdit")
            form = new busForm("edit");
        form->show();
        form->setObjectName("busForm");
        QObject::connect(form, SIGNAL(dataChanged()), this, SLOT(refresh()));
    }
    else if (send.startsWith("control"))
    {
        controlForm* form;
        if (send == "controlAdd")
            form = new controlForm("add");
        if (send == "controlRemove")
            form = new controlForm("remove");
        form->show();
        form->setObjectName("controlForm");
        QObject::connect(form, SIGNAL(dataChanged()), this, SLOT(refresh()));
    }
    else if (send.startsWith("other"))
    {
        otherForm* form;
        if (send == "otherChoice")
            form = new otherForm(tableNum);
        form->show();
        form->setObjectName("otherForm");
        QObject::connect(form, SIGNAL(setQuery(QSqlQuery*)), this, SLOT(setTabQuery(QSqlQuery*)));
        QObject::connect(form, SIGNAL(defaultQuery()), this, SLOT(setTabQuery()));
    }
    else if (send.startsWith("salary"))
    {
        salaryForm* form;
        if (send == "salaryAdd")
            form = new salaryForm("add");
        if (send == "salaryRemove")
            form = new salaryForm("remove");
        if (send == "salaryEdit")
            form = new salaryForm("edit");
        form->show();
        form->setObjectName("salaryForm");
        QObject::connect(form, SIGNAL(dataChanged()), this, SLOT(refresh()));
    }
    else if (send.startsWith("type"))
    {
        typeForm* form;
        if (send == "typeAdd")
            form = new typeForm("add");
        if (send == "typeRemove")
            form = new typeForm("remove");
        if (send == "typeEdit")
            form = new typeForm("edit");
        form->show();
        form->setObjectName("typeForm");
        QObject::connect(form, SIGNAL(dataChanged()), this, SLOT(refresh()));
    }
}

void TableMenu::createQuery(qint32 tab)
{
    qDebug() << "TableMenu createQuery enter";
    bool ok = true;
    QSqlQuery* query = new QSqlQuery();
    if (tab == 0)
    {
        tabModels[tab]->clear();
        query->prepare("select * from route");
        ok = query->exec();
        if (!ok)
        {
            qDebug() << query->lastError().text();
            createMessage("tableMenu", 1);
            emit backButtonClick();
        }
        tabModels[tab]->setQuery(*query);
        tabModels[tab]->setHeaderData(0, Qt::Horizontal, "Номер");
        tabModels[tab]->setHeaderData(1, Qt::Horizontal, "Начало движения");
        tabModels[tab]->setHeaderData(2, Qt::Horizontal, "Окончание движения");
        tabModels[tab]->setHeaderData(3, Qt::Horizontal, "Интервал");
        tabModels[tab]->setHeaderData(4, Qt::Horizontal, "Протяженность");
    }
    else if (tab == 1)
    {
        query->prepare("select driver.*, salary.salary from driver, salary where (driver.class = salary.class and driver.exp = salary.exp)");
        ok = query->exec();
        if (!ok)
        {
            qDebug() << query->lastError().text();
            createMessage("tableMenu", 1);
            emit backButtonClick();
        }
        tabModels[tab]->setQuery(*query);
        tabModels[tab]->setHeaderData(0, Qt::Horizontal, "ID");
        tabModels[tab]->setHeaderData(1, Qt::Horizontal, "ФИО");
        tabModels[tab]->setHeaderData(2, Qt::Horizontal, "Класс");
        tabModels[tab]->setHeaderData(3, Qt::Horizontal, "Стаж");
        tabModels[tab]->setHeaderData(4, Qt::Horizontal, "Зарплата");
    }
    else if (tab == 2)
    {
        query->prepare("select bus.*, bus_type.capacity from bus, bus_type where bus.type = bus_type.type");
        ok = query->exec();
        if (!ok)
        {
            qDebug() << query->lastError().text();
            createMessage("tableMenu", 1);
            emit backButtonClick();
        }
        tabModels[tab]->setQuery(*query);
        tabModels[tab]->setHeaderData(0, Qt::Horizontal, "Номер");
        tabModels[tab]->setHeaderData(1, Qt::Horizontal, "Маршрут");
        tabModels[tab]->setHeaderData(2, Qt::Horizontal, "Тип");
        tabModels[tab]->setHeaderData(3, Qt::Horizontal, "Вместимость");
    }
    else if (tab == 3)
    {
        query->prepare("select control.driver_id, driver.full_name, control.bus_id, bus.route_id from control, driver, bus where driver.driver_id = control.driver_id and bus.bus_id = control.bus_id");
        ok = query->exec();
        if (!ok)
        {
            qDebug() << query->lastError().text();
            createMessage("tableMenu", 1);
            emit backButtonClick();
        }
        tabModels[tab]->setQuery(*query);
        tabModels[tab]->setHeaderData(0, Qt::Horizontal, "ИД водителя");
        tabModels[tab]->setHeaderData(1, Qt::Horizontal, "ФИО");
        tabModels[tab]->setHeaderData(2, Qt::Horizontal, "Номер автобуса");
        tabModels[tab]->setHeaderData(3, Qt::Horizontal, "Маршрут");
    }
    else if (tab == 4)
    {
        if (tabQuery != nullptr)
        {
            ok = tabQuery->exec();
            if (!ok)
            {
                qDebug() << tabQuery->lastError().text();
                createMessage("tableMenu", 1);
                emit backButtonClick();
            }
        }
        if (prevTableNum != tableNum || tabQuery == nullptr)
        {
            if (tableNum == 0)
            {
                query->prepare("select control.bus_id, bus.route_id, control.driver_id, driver.full_name from control, bus, driver where control.bus_id = bus.bus_id and driver.driver_id = control.driver_id order by bus.bus_id");
                ok = query->exec();
                if (!ok)
                {
                    qDebug() << query->lastError().text();
                    createMessage("tableMenu", 1);
                    emit backButtonClick();
                }
                tabModels[tab]->setQuery(*query);
                tabModels[tab]->setHeaderData(0, Qt::Horizontal, "Номер автобуса");
                tabModels[tab]->setHeaderData(1, Qt::Horizontal, "Маршрут");
                tabModels[tab]->setHeaderData(2, Qt::Horizontal, "ИД водителя");
                tabModels[tab]->setHeaderData(3, Qt::Horizontal, "ФИО");
            }
            else if (tableNum == 1)
            {
                query->prepare("select route.route_id, bus.bus_id from route, bus where route.route_id = bus.route_id order by route.route_id");
                ok = query->exec();
                if (!ok)
                {
                    qDebug() << query->lastError().text();
                    createMessage("tableMenu", 1);
                    emit backButtonClick();
                }
                tabModels[tab]->setQuery(*query);
                tabModels[tab]->setHeaderData(0, Qt::Horizontal, "Маршрут");
                tabModels[tab]->setHeaderData(1, Qt::Horizontal, "Номер автобуса");
            }
            else if (tableNum == 2)
            {
                query->prepare("select R.route_id, D.driver_id, D.full_name from route R, driver D where D.driver_id in "
                               "(select driver_id from control C where C.bus_id in (select bus_id from bus B where B.route_id = R.route_id)) order by R.route_id");
                ok = query->exec();
                if (!ok)
                {
                    qDebug() << query->lastError().text();
                    createMessage("tableMenu", 1);
                    emit backButtonClick();
                }
                tabModels[tab]->setQuery(*query);
                tabModels[tab]->setHeaderData(0, Qt::Horizontal, "Маршрут");
                tabModels[tab]->setHeaderData(1, Qt::Horizontal, "ИД водителя");
                tabModels[tab]->setHeaderData(2, Qt::Horizontal, "ФИО");
            }
        }
    }
    else if (tab == 5)
    {
        query->prepare("select class, exp, salary from salary order by class, exp");
        ok = query->exec();
        if (!ok)
        {
            qDebug() << query->lastError().text();
            createMessage("tableMenu", 1);
            emit backButtonClick();
        }
        tabModels[tab]->setQuery(*query);
        tabModels[tab]->setHeaderData(0, Qt::Horizontal, "Класс");
        tabModels[tab]->setHeaderData(1, Qt::Horizontal, "Стаж");
        tabModels[tab]->setHeaderData(2, Qt::Horizontal, "Зарплата");
    }
    else if (tab == 6)
    {
        query->prepare("select type, capacity from bus_type order by type");
        ok = query->exec();
        if (!ok)
        {
            qDebug() << query->lastError().text();
            createMessage("tableMenu", 1);
            emit backButtonClick();
        }
        tabModels[tab]->setQuery(*query);
        tabModels[tab]->setHeaderData(0, Qt::Horizontal, "Тип");
        tabModels[tab]->setHeaderData(1, Qt::Horizontal, "Вместимость");
    }
    qDebug() << "TableMenu createQuery exit";
}

void TableMenu::refresh(int tab)
{
    QString send = sender()->objectName();
    if (send == "routeForm")
        createQuery(0);
    if (send == "driverForm")
        createQuery(1);
    if (send == "busForm")
        createQuery(2);
    if (send == "controlForm")
        createQuery(3);
    if (send == "choiceForm")
        createQuery(4);
    if (send == "salaryForm")
        createQuery(5);
    if (send == "typeForm")
        createQuery(6);
    if (tab != -1)
        createQuery(tab);
}

void TableMenu::tableChoice(int tabN)
{
    qDebug() << tabN << " button cliicked";
    prevTableNum = tableNum;
    tableNum = tabN;
    createQuery(4);
}

void TableMenu::setTabQuery(QSqlQuery* q)
{
    if (q == nullptr)
    {
        if (tabQuery != nullptr)
        {
            delete tabQuery;
            tabQuery = nullptr;
        }
        createQuery(4);
    }
    else
    {
        prevTableNum = tableNum;
        if (tabQuery != nullptr)
            delete tabQuery;
        tabQuery = q;
        bool ok = true;
        if (tableNum == 0)
        {
            ok = tabQuery->exec();
            if (!ok)
            {
                qDebug() << tabQuery->lastError().text();
                createMessage("tableMenu", 1);
                emit backButtonClick();
            }
            tabModels[4]->setQuery(*tabQuery);
            tabModels[4]->setHeaderData(0, Qt::Horizontal, "Номер автобуса");
            tabModels[4]->setHeaderData(1, Qt::Horizontal, "Маршрут");
            tabModels[4]->setHeaderData(2, Qt::Horizontal, "ИД водителя");
            tabModels[4]->setHeaderData(3, Qt::Horizontal, "ФИО");
        }
        else if (tableNum == 1)
        {
            ok = tabQuery->exec();
            if (!ok)
            {
                qDebug() << tabQuery->lastError().text();
                createMessage("tableMenu", 1);
                emit backButtonClick();
            }
            tabModels[4]->setQuery(*tabQuery);
            tabModels[4]->setHeaderData(0, Qt::Horizontal, "Маршрут");
            tabModels[4]->setHeaderData(1, Qt::Horizontal, "Номер автобуса");
        }
        else if (tableNum == 2)
        {
            ok = tabQuery->exec();
            if (!ok)
            {
                qDebug() << tabQuery->lastError().text();
                createMessage("tableMenu", 1);
                emit backButtonClick();
            }
            tabModels[4]->setQuery(*tabQuery);
            tabModels[4]->setHeaderData(0, Qt::Horizontal, "Маршрут");
            tabModels[4]->setHeaderData(1, Qt::Horizontal, "ИД водителя");
            tabModels[4]->setHeaderData(2, Qt::Horizontal, "ФИО");
        }
    }
}

void TableMenu::createReport()
{
    bool ok = true;
    QFile report("report.txt");
    ok = report.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    if (!ok)
    {
        createMessage("tableMenu", 2);
        return;
    }
    QTextStream stream(&report);
    //stream.setCodec("UTF-8");

    stream << tr("Отчет по автопарку") << endl << endl;
    QSqlQuery query;
    query.prepare("select count(*) from bus");
    ok = query.exec();
    if (!ok)
    {
        createMessage("tableMenu", 1);
        return;
    }
    query.next();
    qint32 count = query.value(0).toInt();
    stream << tr("Количество автобусов: ") << count << endl;
    if (count != 0)
    {
        query.prepare("select type from bus_type");
        ok = query.exec();
        if (!ok)
        {
            qDebug() << "select type " << query.lastError().text();
            createMessage("tableMenu", 1);
            return;
        }
        for (qint32 i = 0; i < query.size(); ++i)
        {
            query.next();
            QSqlQuery countQuery;
            countQuery.prepare("select count(*) from bus where type = ?");
            countQuery.bindValue(0, query.value(0).toInt());
            ok = countQuery.exec();
            if (!ok)
            {
                qDebug() << "select count type #" << i << " " << query.lastError().text();
                createMessage("tableMenu", 1);
                return;
            }
            countQuery.next();
            if (countQuery.value(0).toInt() != 0)
                stream << tr("\tТип: ") << query.value(0).toInt() << tr(" количество: ") << countQuery.value(0).toInt() << endl;
        }
    }
    stream << endl;
    query.prepare("select * from route");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << "select route " << query.lastError().text();
        createMessage("tableMenu", 1);
        return;
    }
    count = query.size();
    stream << tr("Количество маршрутов: ") << count << endl;
    if (count != 0)
    {
        for (qint32 i = 0; i < count; ++i)
        {
            query.next();
            stream << tr("\t№") << query.value(0).toInt() << tr(" Время движения: ") << query.value(1).toString() << "-" <<
                      query.value(2).toString() << tr(" интервал: ") << query.value(3).toInt() << tr(" протяженность: ") <<
                      query.value(4).toInt() << endl;
        }
    }
    stream << endl;
    query.prepare("select * from driver");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << "select driver " << query.lastError().text();
        createMessage("tableMenu", 1);
        return;
    }
    count = query.size();
    stream << tr("Количество водителей: ") << count << endl;
    if (count != 0)
    {
        for (qint32 i = 0; i < count; ++i)
        {
            query.next();
            stream << tr("\tИД водителя: ") << query.value(0).toInt() << tr(" ФИО: ") << query.value(1).toString() << tr(" класс: ") <<
                      query.value(2).toInt() << tr(" стаж: ") << query.value(3).toInt() << endl;
        }
    }
    report.close();
    createMessage("tableMenu", 3);
}
