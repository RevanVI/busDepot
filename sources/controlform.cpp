#include "controlform.h"
#include "ui_controlform.h"

controlForm::controlForm(const QString& Mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controlForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    mode = Mode;
    if (mode == "add")
        this->setWindowTitle("Добавление данных");
    if (mode == "remove")
        this->setWindowTitle("Удаление данных");
    if (mode == "edit")
        this->setWindowTitle("Изменение данных");

    setWindowModality(Qt::ApplicationModal);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    //установка значений по умолчанию
    setDefaultInfo();

    getControlList();
    if (mode == "remove")
    {
        getDrivingBus();
        QObject::connect(ui->driverCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getDrivingBus()));
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(removeControl()));
    }
    else
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(addControl()));
}

controlForm::~controlForm()
{
    delete ui;
}

bool controlForm::getControlList()
{
    qDebug() << "controlForm getControlList enter";
    if (ui->driverCh->count() != 0)
        ui->driverCh->clear();
    bool ok = true;
    QSqlQuery query;
    if (mode == "add")
        query.prepare("select driver_id, full_name from driver");
    else //mode == "remove"
        query.prepare("select distinct control.driver_id, driver.full_name from control, driver where control.driver_id = driver.driver_id");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("controlForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->driverCh->addItem(query.value(0).toString() + " " + query.value(1).toString());
    }

    qDebug() << "controlForm getControlList exit";
    return ok;
}

void controlForm::addControl()
{
    qDebug() << "controlForm addControlInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select bus_id from control where driver_id = ?");
    query.bindValue(0, ui->driverCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("controlForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        if (ui->busCh->currentText().section(' ', 0, 0).toInt() == query.value(0).toInt())
        {
            ok = false;
            createMessage("controlForm", 2, ui->busCh->currentText().section(' ', 0, 0).toInt());
            return;
        }
    }
    query.prepare("insert into control (driver_id, bus_id) values (?, ?)");
    query.bindValue(0, ui->driverCh->currentText().section(' ', 0, 0).toInt());
    query.bindValue(1, ui->busCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("controlForm", 1);
        this->close();
    }
    if (ok)
    {
        emit dataChanged();
        setDefaultInfo();
    }
    qDebug() << "controlForm addControlInfo exit";
}

void controlForm::setDefaultInfo()
{
    QSqlQuery query;
    query.prepare("select bus_id, route_id from bus");
    bool ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("controlForm", 1);
        this->close();
    }
    if (ui->busCh->count() != 0)
        ui->busCh->clear();
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->busCh->addItem(query.value(0).toString()+ " №" + query.value(1).toString());
    }
    ui->busCh->setCurrentIndex(0);
}

void controlForm::removeControl()
{
    bool ok = true;
    QSqlQuery query;
    query.prepare("delete from control where driver_id = ? and bus_id = ?");
    query.bindValue(0, ui->driverCh->currentText().section(' ', 0, 0).toInt());
    query.bindValue(1, ui->busCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("controlForm", 1);
        this->close();
    }
    emit dataChanged();
    getControlList();
}

void controlForm::getDrivingBus()
{
    qDebug() << "controlForm getDrivingBus enter";
    if (ui->busCh->count() != 0)
        ui->busCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select distinct control.bus_id, bus.route_id from control, bus where control.driver_id = ? and control.bus_id = bus.bus_id");
    query.bindValue(0, ui->driverCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("controlForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->busCh->addItem(query.value(0).toString() + " №" + query.value(1).toString());
    }

    qDebug() << "controlForm getDrivingBus exit";
}
