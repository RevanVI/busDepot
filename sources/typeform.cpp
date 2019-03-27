#include "typeform.h"
#include "ui_typeform.h"

typeForm::typeForm(const QString &Mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::typeForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    mode = Mode;
    if (mode == "add")
    {
        this->setWindowTitle("Добавление данных");
        ui->typeCh->hide();
    }
    if (mode == "remove")
    {
        this->setWindowTitle("Удаление данных");
        ui->typeEdit->hide();
        ui->label_2->hide();
        ui->capEdit->hide();
    }
    if (mode == "edit")
    {
        this->setWindowTitle("Изменение данных");
        ui->typeEdit->hide();
    }

    setWindowModality(Qt::ApplicationModal);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    ui->typeEdit->setMinimum(1);
    ui->typeEdit->setMaximum(100);
    ui->typeEdit->setSingleStep(1);
    ui->capEdit->setMinimum(0);
    ui->capEdit->setMaximum(500);
    ui->capEdit->setSingleStep(1);

    //установка значений по умолчанию
    setDefaultInfo();

    getTypeList();
    if (mode == "edit")
    {
        QObject::connect(ui->typeCh, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(getCapacityInfo()));
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(setCapacityInfo()));
        getCapacityInfo();
    }
    else if (mode == "remove")
    {
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(removeType()));
    }
    else
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(addType()));
}

typeForm::~typeForm()
{
    delete ui;
}

void typeForm::getTypeList()
{
    qDebug() << "typeForm getTypeList enter";
    if (ui->typeCh->count() != 0)
        ui->typeCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select type from bus_type");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("typeForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->typeCh->addItem(query.value(0).toString());
    }

    ui->typeCh->setCurrentIndex(0);
    qDebug() << "typeForm getTypeList exit";
}

void typeForm::getCapacityInfo()
{
    qDebug() << "typeForm getTypeInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select capacity from bus_type where type = ?");
    query.bindValue(0, ui->typeCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("typeForm", 1);
        this->close();
    }
    query.next();
    ui->capEdit->setValue(query.value(0).toInt());
    qDebug() << "typeForm getCapacityInfo exit";
}

void typeForm::setCapacityInfo()
{
    qDebug() << "typeForm setCapacityInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("update bus_type set capacity = ? where type = ?");
    query.bindValue(0, ui->capEdit->value());
    query.bindValue(1, ui->typeCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("typeForm", 1);
        this->close();
    }
    if (ok)
    {
        emit dataChanged();
        getTypeList();
        getCapacityInfo();
    }
    qDebug() << "typeForm setTypeInfo exit";
}

void typeForm::addType()
{
    qDebug() << "typeForm addType enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select count(*) from bus_type  where type = ?");
    query.bindValue(0, ui->typeEdit->value());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("typeForm", 1);
        this->close();
    }
    query.next();
    if (query.value(0).toInt() != 0)
    {
        createMessage("typeForm", 3);
        return;
    }
    query.prepare("insert into bus_type (type, capacity) values (?, ?)");
    query.bindValue(0, ui->typeEdit->value());
    query.bindValue(1, ui->capEdit->value());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("typeForm", 1);
        this->close();
    }
    emit dataChanged();
    setDefaultInfo();
    qDebug() << "typeForm addType exit";
}

void typeForm::setDefaultInfo()
{
    ui->typeEdit->setValue(1);
    ui->capEdit->setValue(30);
}

void typeForm::removeType()
{
    qDebug() << "typeForm deleteType enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select count(*) from bus where type = ?");
    query.bindValue(0, ui->typeCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("typeForm", 1);
        this->close();
    }
    query.next();
    if (query.value(0) != 0)
    {
        createMessage("typeForm", 2);
        return;
    }
    query.prepare("delete from bus_type where  type = ?");
    query.bindValue(0, ui->typeCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("typeForm", 1);
        this->close();
    }
    emit dataChanged();
    getTypeList();
    qDebug() << "typeForm deleteType exit";
}
