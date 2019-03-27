#include "driverform.h"
#include "ui_driverform.h"

driverForm::driverForm(const QString &Mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::driverForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    mode = Mode;
    if (mode == "add")
    {
        this->setWindowTitle("Добавление водителя");
        ui->label->hide();
        ui->driverCh->hide();
    }
    if (mode == "remove")
    {
        this->setWindowTitle("Удаление водителя");
        ui->gridLayoutWidget->hide();
    }
    if (mode == "edit")
        this->setWindowTitle("Изменение данных о водителе");


    QObject::connect(ui->classCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getExpList()));
    QObject::connect(ui->expCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getSalary()));
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    if (mode != "add")
    {
        getDriverList();
        if (mode == "edit")
        {
            getDriverInfo();
            QObject::connect(ui->driverCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getDriverInfo()));
            QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(setDriverInfo()));
        }
        else
            QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(removeDriver()));
    }
    else
    {
        setDefaultInfo();
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(addDriver()));
    }
}

driverForm::~driverForm()
{
    delete ui;
}

void driverForm::getDriverList()
{
    qDebug() << "driverForm getDriverList enter";
    QObject::disconnect(ui->driverCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getDriverInfo()));
    if (ui->driverCh->count() != 0)
        ui->driverCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select driver_id, full_name from driver");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->driverCh->addItem(query.value(0).toString() + " " + query.value(1).toString());
    }
    ui->driverCh->setCurrentIndex(1);
    QObject::connect(ui->driverCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getDriverInfo()));
    ui->driverCh->setCurrentIndex(0);
    qDebug() << "driverForm getDriverList exit";
}

void driverForm::getDriverInfo()
{
    qDebug() << "driverForm getDriverInfo enter";
    bool ok = true;

    qint32 id = ui->driverCh->currentText().section(' ', 0, 0).toInt();
    QSqlQuery query;
    query.prepare("select full_name from driver where driver_id = ?");
    query.bindValue(0, id);
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    query.next();
    ui->fullNameEdit->setText(query.value(0).toString());

    getClassList();
    qDebug() << "driverForm getDriverInfo exit";
}

void driverForm::getClassList()
{
    qDebug() << "driverForm getClassList enter";
    QObject::disconnect(ui->classCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getExpList()));
    if (ui->classCh->count() != 0)
        ui->classCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select distinct class from salary");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->classCh->addItem(query.value(0).toString());
    }
    query.prepare("select class from driver where driver_id = ?");
    query.bindValue(0, ui->driverCh->currentText().section(' ', 0, 0));
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    query.next();
    qint32 ind = 0;
    for (qint32 i = 0; i < ui->classCh->count(); ++i)
    {
        if (query.value(0) == ui->classCh->itemText(i).toInt())
        {
            ind = i;
            break;
        }
    }
    ui->classCh->setCurrentIndex(ind);
    QObject::connect(ui->classCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getExpList()));
    getExpList();
    qDebug() << "driverForm getClassList exit";
}

void driverForm::getExpList()
{
    qDebug() << "driverForm getExpList enter";
    QObject::disconnect(ui->expCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getSalary()));
    if (ui->expCh->count() != 0)
        ui->expCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select exp from salary where class = ?");
    query.bindValue(0, ui->classCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    qDebug() << "create expCh " << query.size();
    for (qint32 i = 0; i < query.size(); ++i)
    {
        qDebug() << "step " << i;
        query.next();
        ui->expCh->addItem(query.value(0).toString());
    }
    qDebug() << "search exp from driver";
    query.prepare("select exp from driver where driver_id = ?");
    query.bindValue(0, ui->driverCh->currentText().section(' ', 0, 0));
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    qint32 ind = 0;
    if (query.size() != 0)
    {
        query.next();
        for (qint32 i = 0; i < ui->expCh->count(); ++i)
        {
            if (query.value(0) == ui->expCh->itemText(i).toInt())
            {
                ind = i;
                break;
            }
        }
    }
    ui->expCh->setCurrentIndex(ind);
    QObject::connect(ui->expCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getSalary()));
    getSalary();
    qDebug() << "driverForm getExpList exit";
}

void driverForm::getSalary()
{
    qDebug() << "driverForm getSalary enter";
    bool ok = true;
    QString curClass = ui->classCh->currentText();
    QString curExp = ui->expCh->currentText();
    QSqlQuery query;
    query.prepare("select salary from salary where class = ? and exp = ?");
    query.bindValue(0, curClass.toInt());
    query.bindValue(1, curExp.toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    query.next();
    ui->salaryLbl->setText(query.value(0).toString());
    qDebug() << "driverForm getSalary exit";
}

void driverForm::setDriverInfo()
{
    qDebug() << "driverForm setDriverInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("update driver set full_name = ?, class = ?, exp = ? where driver_id = ?");
    query.bindValue(0, ui->fullNameEdit->text());
    query.bindValue(1, ui->classCh->currentText().toInt());
    query.bindValue(2, ui->expCh->currentText().toInt());
    query.bindValue(3, ui->driverCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    if (ok)
    {
        emit dataChanged();
        if (mode == "edit")
        {
            QObject::disconnect(ui->driverCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getDriverInfo()));
            this->getDriverList();
            getDriverInfo();
            QObject::connect(ui->driverCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getDriverInfo()));
        }
    }
    qDebug() << "driverForm setDriverInfo exit";
}

void driverForm::addDriver()
{
    qDebug() << "driverForm addDriverInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("insert into driver (full_name, class, exp) values (?, ?, ?)");
    query.bindValue(0, ui->fullNameEdit->text());
    query.bindValue(1, ui->classCh->currentText().toInt());
    query.bindValue(2, ui->expCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    if (ok)
    {
        setDefaultInfo();
        emit dataChanged();
    }
    qDebug() << "driverForm addDriverInfo exit";
}

void driverForm::setDefaultInfo()
{
    ui->fullNameEdit->setText("Name Surname");
    QObject::disconnect(ui->classCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getExpList()));
    if (ui->classCh->count() != 0)
        ui->classCh->clear();
    QSqlQuery query;
    query.prepare("select distinct class from salary");
    bool ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->classCh->addItem(query.value(0).toString());
    }
    ui->classCh->setCurrentIndex(0);
    QObject::connect(ui->classCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getExpList()));
    QObject::disconnect(ui->expCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getSalary()));
    if (ui->expCh->count() != 0)
        ui->expCh->clear();
    ok = true;
    query.prepare("select exp from salary where class = ?");
    query.bindValue(0, ui->classCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->expCh->addItem(query.value(0).toString());
    }
    QObject::connect(ui->expCh, SIGNAL(currentIndexChanged(int)), this, SLOT(getSalary()));
    getSalary();
}

void driverForm::removeDriver()
{
    bool ok = true;
    QSqlQuery query;
    query.prepare("delete from driver where driver_id = ?");
    query.bindValue(0, ui->driverCh->currentText().section(' ', 0, 0).toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("driverForm", 1);
        this->close();
    }
    emit dataChanged();
    getDriverList();
}

