#include "busform.h"
#include "ui_busform.h"

busForm::busForm(const QString &Mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    mode = Mode;
    if (mode == "add")
    {
        this->setWindowTitle("Добавление автобуса");
        ui->label_4->hide();
        ui->busCh->hide();
    }
    if (mode == "remove")
    {
        this->setWindowTitle("Удаление автобуса");
        ui->gridLayoutWidget->hide();
    }
    if (mode == "edit")
        this->setWindowTitle("Изменение данных о автобусе");

    setWindowModality(Qt::ApplicationModal);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    //установка значений по умолчанию
    QObject::connect(ui->typeCh, SIGNAL(currentIndexChanged(int)), this, SLOT(setCapacity()));
    setDefaultInfo();


    getBusList();
    if (mode == "edit")
    {
        getBusInfo();
        QObject::connect(ui->busCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getBusInfo()));
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(setBusInfo()));
    }
    else if (mode == "remove")
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(removeBus()));
    else
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(addBus()));
}

busForm::~busForm()
{
    delete ui;
}

bool busForm::getBusList()
{
    qDebug() << "busForm getBusList enter";
    if (ui->busCh->count() != 0)
        ui->busCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select bus_id, route_id from bus");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->busCh->addItem(query.value(0).toString() + " " + query.value(1).toString());
    }

    qDebug() << "busForm getBusList exit";
    return ok;
}

void busForm::getBusInfo()
{
    qDebug() << "busForm getBusInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select bus.route_id, bus.type, bus_type.capacity from bus, bus_type where bus.bus_id = ? and bus.type = bus_type.type");
    query.bindValue(0, ui->busCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    query.next();
    qint32 index = 0;
    for (qint32 i = 0; i < ui->routeCh->count(); ++i)
    {
        if (query.value(0).toInt() == ui->routeCh->itemText(i).toInt())
        {
            index = i;
            break;
        }
    }
    ui->routeCh->setCurrentIndex(index);
    for (qint32 i = 0; i < ui->typeCh->count(); ++i)
    {
        if (query.value(1).toInt() == ui->typeCh->itemText(i).toInt())
        {
            index = i;
            break;
        }
    }
    ui->typeCh->setCurrentIndex(index);
    qDebug() << "busForm getBusInfo exit";
}

void busForm::setBusInfo()
{
    qDebug() << "busForm setBusInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("update bus set route_id = ?, type = ? where bus_id = ?");
    query.bindValue(0, ui->routeCh->currentText().toInt());
    query.bindValue(1, ui->typeCh->currentText().toInt());
    query.bindValue(2, ui->busCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    if (ok)
    {
        emit dataChanged();
        QObject::disconnect(ui->busCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getBusInfo()));
        getBusList();
        getBusInfo();
        QObject::connect(ui->busCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getBusInfo()));
    }
    qDebug() << "busForm setBusInfo exit";
}

void busForm::setDefaultInfo()
{
    qDebug() << "busForm setDefaultInfo enter";
    QSqlQuery query;
    if (ui->routeCh->count() != 0)
        ui->routeCh->clear();
    query.prepare("select route_id from route");
    bool ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    qDebug() << "create routeCh";
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->routeCh->addItem(query.value(0).toString());
    }
    qDebug() << "clear typeCh";
    QObject::disconnect(ui->typeCh, SIGNAL(currentIndexChanged(int)), this, SLOT(setCapacity()));
    if (ui->typeCh->count() != 0)
        ui->typeCh->clear();
    query.prepare("select type from bus_type");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    qDebug() << "create typeCh";
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->typeCh->addItem(query.value(0).toString());
    }
    QObject::connect(ui->typeCh, SIGNAL(currentIndexChanged(int)), this, SLOT(setCapacity()));
    ui->routeCh->setCurrentIndex(0);
    ui->typeCh->setCurrentIndex(1);
    ui->typeCh->setCurrentIndex(0);
    qDebug() << "busForm setDefaultInfo exit";
}

void busForm::addBus()
{
    qDebug() << "busForm addBusInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("insert into bus (route_id, type) values (?, ?)");
    query.bindValue(0, ui->routeCh->currentText().toInt());
    query.bindValue(1, ui->typeCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    if (ok)
    {
        setDefaultInfo();
        emit dataChanged();
    }
    qDebug() << "busForm addBusInfo exit";
}

void busForm::removeBus()
{
    bool ok = true;
    QSqlQuery query;
    query.prepare("select count(*) from control where bus_id = ?");
    query.bindValue(0, ui->busCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 2, ui->busCh->currentText().section(' ', 0, 0).toInt());
        this->close();
    }
    query.prepare("delete from bus where bus_id = ?");
    query.bindValue(0, ui->busCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    emit dataChanged();
    getBusList();
}

void busForm::setCapacity()
{
    qDebug() << "busForm setCapacity enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select capacity from bus_type where type = ?");
    query.bindValue(0, ui->typeCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("busForm", 1);
        this->close();
    }
    query.next();
    ui->capLbl->setText(query.value(0).toString());
    qDebug() << "busForm setCapacity exit";
}
