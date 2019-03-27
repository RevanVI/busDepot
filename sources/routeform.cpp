#include "routeform.h"
#include "ui_routeform.h"

routeForm::routeForm(const QString& Mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::routeForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    mode = Mode;
    if (mode == "add")
    {
        this->setWindowTitle("Добавление маршрута");
        ui->numberLbl->hide();
        ui->routeCh->hide();
    }
    if (mode == "remove")
    {
        this->setWindowTitle("Удаление маршрута");
        ui->gridLayoutWidget->hide();
    }
    if (mode == "edit")
        this->setWindowTitle("Изменение маршрута");

    setWindowModality(Qt::ApplicationModal);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    //установка значений по умолчанию
    ui->routeCr->setMinimum(1);
    ui->routeCr->setMaximum(999);
    ui->tEdit->setMinimum(1);
    ui->tEdit->setSingleStep(1);
    ui->sEdit->setMinimum(0.0);
    ui->sEdit->setSingleStep(0.1);
    setDefaultInfo();

    getRouteList();
    if (mode == "edit")
    {
        getRouteInfo();
        QObject::connect(ui->routeCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getRouteInfo()));
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(setRouteInfo()));
    }
    else if (mode == "remove")
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(removeRoute()));
    else
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(addRoute()));
}

routeForm::~routeForm()
{
    qDebug() << "routeForm deleted";
    delete ui;
}

bool routeForm::getRouteList()
{
    qDebug() << "routeForm getRouteList enter";
    if (ui->routeCh->count() != 0)
        ui->routeCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select route_id from route");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("routeForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->routeCh->addItem(query.value(0).toString());
    }
    qDebug() << "routeForm getRouteList exit";
    return ok;
}

void routeForm::getRouteInfo()
{
    qDebug() << "routeForm getRouteInfo enter";
    qint32 id = ui->routeCh->currentText().toInt();

    bool ok;
    QSqlQuery query;
    query.prepare("select * from route where route_id = ?");
    query.bindValue(0, id);
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("routeForm", 1);
        this->close();
    }
    query.next();
    ui->routeCr->setValue(id);
    ui->tsEdit->setTime(query.value(1).toTime());
    ui->teEdit->setTime(query.value(2).toTime());
    ui->tEdit->setValue(query.value(3).toInt());
    ui->sEdit->setValue(query.value(4).toDouble());
    qDebug() << "routeForm getRouteInfo exit";
}

void routeForm::setRouteInfo()
{
    bool ok = true;
    QSqlQuery query;
    if (ui->routeCh->currentText().toInt() == ui->routeCr->value())
    {
        query.prepare("update route set ts = ?, te = ?, t = ?, length = ? where route_id = ?");
        query.bindValue(0, ui->tsEdit->time());
        query.bindValue(1, ui->teEdit->time());
        query.bindValue(2, ui->tEdit->value());
        query.bindValue(3, ui->sEdit->value());
        query.bindValue(4, ui->routeCh->currentText().toInt());
        ok = query.exec();
        if (!ok)
        {
            qDebug() << query.lastError().text();
            createMessage("routeForm", 1);
            this->close();
        }
    }
    else
    {
        ok = isUniqueIds();
        if (!ok)
            return;
        else
        {
            query.prepare("update route set ts = ?, te = ?, t = ?, length = ? where route_id = ?");
            query.bindValue(0, ui->tsEdit->time());
            query.bindValue(1, ui->teEdit->time());
            query.bindValue(2, ui->tEdit->value());
            query.bindValue(3, ui->sEdit->value());
            query.bindValue(4, ui->routeCh->currentText().toInt());
            ok = query.exec();
            if (!ok)
            {
                qDebug() << query.lastError().text();
                createMessage("routeForm", 1);
                this->close();
            }
            query.prepare("update route set route_id = ? where route_id = ?");
            query.bindValue(0, ui->routeCr->value());
            query.bindValue(1, ui->routeCh->currentText().toInt());
            ok = query.exec();
            if (!ok)
            {
                qDebug() << query.lastError().text();
                createMessage("routeForm", 1);
                this->close();
            }
        }
    }
    if (ok)
    {
        emit dataChanged();
        if (mode == "edit")
        {
            QObject::disconnect(ui->routeCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getRouteInfo()));
            this->getRouteList();
            getRouteInfo();
            QObject::connect(ui->routeCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getRouteInfo()));
        }
        else
            this->getRouteInfo();
    }
}

void routeForm::addRoute()
{
    bool ok = isUniqueIds();
    if (!ok)
        return;

    QSqlQuery query;
    query.prepare("select route_id from route where route_id = ?");
    query.bindValue(0, ui->routeCr->value());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("routeForm", 1);
        this->close();
    }
    if (query.size() != 0)
    {
        createMessage("routeForm", 3);
        return;
    }
    query.prepare("insert into route values (?, ?, ?, ?, ?)");
    query.bindValue(0, ui->routeCr->value());
    query.bindValue(1, ui->tsEdit->time());
    query.bindValue(2, ui->teEdit->time());
    query.bindValue(3, ui->tEdit->value());
    query.bindValue(4, ui->sEdit->value());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("routeForm", 1);
        this->close();
    }
    else
    {
        emit dataChanged();
        setDefaultInfo();
    }
}

void routeForm::setDefaultInfo()
{
    ui->routeCr->setValue(1);
    ui->tsEdit->setTime(QVariant("07:00:00").toTime());
    ui->teEdit->setTime(QVariant("23:00:00").toTime());
    ui->tEdit->setValue(1);
    ui->sEdit->setValue(1.0);
}

bool routeForm::isUniqueIds()
{
    bool ok = true;
    for (qint32 i = 0; i < ui->routeCh->count(); ++i)
        if (ui->routeCr->value() == ui->routeCh->itemText(i).toInt())
        {
            ok = false;
            break;
        }
    if (!ok)
    {
        createMessage("routeForm", 3, ui->routeCr->value());
        return false;
    }
    return true;
}

void routeForm::removeRoute()
{
    bool ok = true;
    QSqlQuery query;
    query.prepare("select bus_id from bus where route_id = ?");
    query.bindValue(0, ui->routeCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
        qDebug() << query.lastError().text();
    if (query.size() > 0)
    {
        createMessage("routeInfo", 2, ui->routeCh->currentText().toInt());
        return;
    }
    else
    {
        query.prepare("delete from route where route_id = ?");
        query.bindValue(0, ui->routeCh->currentText().toInt());
        ok = query.exec();
        if (!ok)
        {
            qDebug() << query.lastError().text();
            createMessage("routeForm", 1);
            this->close();
        }
        emit dataChanged();
        getRouteList();
    }
}
