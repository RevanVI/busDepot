#include "otherform.h"
#include "ui_otherform.h"

otherForm::otherForm(qint32 tabN, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::otherForm)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Выбор");

    setWindowModality(Qt::ApplicationModal);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    tabNum = tabN;
    if (tabNum == 0)
        ui->label->setText("Водитель:");
    else
        ui->label->setText("Маршрут:");

    getList();
    QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(createQuery()));
    QObject::connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelQuery()));
}

otherForm::~otherForm()
{
    delete ui;
}

void otherForm::getList()
{
    bool ok = true;
    QSqlQuery query;
    if (tabNum == 0)
    {
        query.prepare("select distinct control.bus_id, bus.route_id from control, bus where control.bus_id = bus.bus_id");
        query.next();
        ok = query.exec();
        if (!ok)
        {
            qDebug() << query.lastError().text();
            createMessage("otherForm", 1);
            this->close();
        }
        for (qint32 i = 0; i < query.size(); ++i)
        {
            query.next();
            ui->comboBox->addItem(query.value(0).toString() + " №" + query.value(1).toString());
        }
    }
    else
    {
        query.prepare("select distinct route_id from route order by route_id");
        query.next();
        ok = query.exec();
        if (!ok)
        {
            qDebug() << query.lastError().text();
            createMessage("otherForm", 1);
            this->close();
        }
        for (qint32 i = 0; i < query.size(); ++i)
        {
            query.next();
            ui->comboBox->addItem(query.value(0).toString() + ' ');
        }
    }
    ui->comboBox->setCurrentIndex(0);
}

void otherForm::createQuery()
{
    bool ok = true;
    qint32 id = ui->comboBox->currentText().section(' ', 0, 0).toInt();
    QSqlQuery * query = nullptr;
    if (tabNum == 0)
    {
        query = new QSqlQuery("select C.bus_id, B.route_id, C.driver_id, D.full_name from control C, bus B, driver D where D.driver_id = C.driver_id and B.bus_id  = C.bus_id and C.bus_id = " + QString::number(id));
    }
    else
    {
        if (tabNum == 1)
            query = new QSqlQuery("select R.route_id, B.bus_id from route R, bus B where R.route_id = B.route_id and R.route_id = ? order by R.route_id");
        else //tabNum == 2
            query = new QSqlQuery("SELECT DISTINCT R.route_id,   C.driver_id, D.full_name FROM route R, driver D, control C, "
                                  "(SELECT B.bus_id, B.route_id FROM bus B WHERE B.route_id = ?) BB "
                                  "WHERE R.route_id = BB.route_id AND C.bus_id = BB.bus_id AND D.driver_id = C.driver_id");
        query->bindValue(0, id);
        ok = query->exec();
        if (!ok)
        {
            qDebug() << query->lastError().text();
            createMessage("otherForm", 1);
            this->close();
        }
        else if (query->size() == 0)
        {
            createMessage("otherForm", 2);
            return;
        }
    }
    emit setQuery(query);
    this->close();
}

void otherForm::cancelQuery()
{
    emit defaultQuery();
    this->close();
}
