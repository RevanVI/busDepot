#include "salaryform.h"
#include "ui_salaryform.h"

salaryForm::salaryForm(const QString& Mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::salaryForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    mode = Mode;
    if (mode == "add")
    {
        this->setWindowTitle("Добавление данных");
        ui->classCh->hide();
        ui->expCh->hide();
    }
    if (mode == "remove")
    {
        this->setWindowTitle("Удаление данных");
        ui->classEdit->hide();
        ui->expEdit->hide();
        ui->salaryEdit->hide();
        ui->label_3->hide();
    }
    if (mode == "edit")
    {
        this->setWindowTitle("Изменение данных");
        ui->classEdit->hide();
        ui->expEdit->hide();
    }

    setWindowModality(Qt::ApplicationModal);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    ui->classEdit->setMinimum(1);
    ui->classEdit->setMaximum(100);
    ui->classEdit->setSingleStep(1);
    ui->expEdit->setMinimum(0);
    ui->expEdit->setMaximum(500);
    ui->expEdit->setSingleStep(1);
    ui->salaryEdit->setMinimum(0);
    ui->salaryEdit->setMaximum(999999);
    ui->salaryEdit->setSingleStep(1000);

    //установка значений по умолчанию
    setDefaultInfo();

    getClassList();
    getExpList();
    if (mode == "edit")
    {
        QObject::connect(ui->classCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getExpList()));
        QObject::connect(ui->expCh, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(getSalaryInfo()));
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(setSalaryInfo()));
        getSalaryInfo();
    }
    else if (mode == "remove")
    {
        QObject::connect(ui->classCh, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(getExpList()));
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(removeSalary()));
    }
    else
        QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(addSalary()));
}

salaryForm::~salaryForm()
{
    delete ui;
}

void salaryForm::getClassList()
{
    qDebug() << "salaryForm getClassList enter";
    if (ui->classCh->count() != 0)
        ui->classCh->clear();
    bool ok = true;
    QSqlQuery query;
    query.prepare("select distinct class from salary");
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("salaryForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->classCh->addItem(query.value(0).toString());
    }

    ui->classCh->setCurrentIndex(0);
    qDebug() << "salaryForm getClassList exit";
}

void salaryForm::getExpList()
{
    qDebug() << "salaryForm getExpList enter";
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
        createMessage("salaryForm", 1);
        this->close();
    }
    for (qint32 i = 0; i < query.size(); ++i)
    {
        query.next();
        ui->expCh->addItem(query.value(0).toString());
    }

    ui->expCh->setCurrentIndex(0);
    qDebug() << "salaryForm getExpList exit";
}

void salaryForm::getSalaryInfo()
{
    qDebug() << "salaryForm getSalaryInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select salary from salary where class = ? and exp = ?");
    query.bindValue(0, ui->classCh->currentText().toInt());
    query.bindValue(1, ui->expCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("salaryForm", 1);
        this->close();
    }
    query.next();
    ui->salaryEdit->setValue(query.value(0).toInt());
    qDebug() << "salaryForm getSalaryInfo exit";
}

void salaryForm::setSalaryInfo()
{
    qDebug() << "salaryForm setSalaryInfo enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("update salary set salary = ? where class = ? and exp = ?");
    query.bindValue(0, ui->salaryEdit->value());
    query.bindValue(1, ui->classCh->currentText().toInt());
    query.bindValue(2, ui->expCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("salaryForm", 1);
        this->close();
    }
    if (ok)
    {
        emit dataChanged();
        getClassList();
        getExpList();
        getSalaryInfo();
    }
    qDebug() << "salaryForm setSalaryInfo exit";
}

void salaryForm::addSalary()
{
    qDebug() << "salaryForm addSalary enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare("select count(*) from salary  where class = ? and exp = ?");
    query.bindValue(0, ui->classEdit->value());
    query.bindValue(1, ui->expEdit->value());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("salaryForm", 1);
        this->close();
    }
    query.next();
    if (query.value(0).toInt() != 0)
    {
        createMessage("salaryForm", 3);
        return;
    }
    query.prepare("insert into salary (class, exp, salary) values (?, ?, ?)");
    query.bindValue(0, ui->classEdit->value());
    query.bindValue(1, ui->expEdit->value());
    query.bindValue(2, ui->salaryEdit->value());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("salaryForm", 1);
        this->close();
    }
    emit dataChanged();
    setDefaultInfo();
    qDebug() << "salaryForm addSalary exit";
}

void salaryForm::setDefaultInfo()
{
    ui->classEdit->setValue(1);
    ui->expEdit->setValue(0);
    ui->salaryEdit->setValue(10000);
}

void salaryForm::removeSalary()
{
    qDebug() << "salaryForm deleteSalary enter";
    bool ok = true;
    QSqlQuery query;
    query.prepare(" select count(*) from driver where class = ? and exp = ?");
    query.bindValue(0, ui->classCh->currentText().toInt());
    query.bindValue(1, ui->expCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("salaryForm", 1);
        this->close();
    }
    query.next();
    if (query.value(0) != 0)
    {
        createMessage("salaryForm", 2);
        return;
    }
    query.prepare("delete from salary where  class = ? and exp = ?");
    query.bindValue(0, ui->classCh->currentText().toInt());
    query.bindValue(1, ui->expCh->currentText().toInt());
    ok = query.exec();
    if (!ok)
    {
        qDebug() << query.lastError().text();
        createMessage("salaryForm", 1);
        this->close();
    }
    emit dataChanged();
    getClassList();
    getExpList();
    qDebug() << "salaryForm deleteSalary exit";
}
