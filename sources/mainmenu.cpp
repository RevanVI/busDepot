#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QFile>
#include <QTextStream>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    QFile file ("log.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream logStream(&file);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("busdepot");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("0000");
    bool ok = db.open();
    if (!ok && db.lastError().type() == 1) //нет базы данных
    {
        logStream << db.lastError().text() << endl;
        QMessageBox* msg = createMessage("mainMenu", 3);
        QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
    }
    QSqlQuery query;
    ok = query.exec("SHOW TABLES");
    if (!ok && db.lastError().type() == 1) //нет базы данных
    {
        QMessageBox* msg = createMessage("mainMenu", 3);
        QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
    }
    if (query.size() == 0)
    {
        //Инициализация БД
        ok = query.exec("CREATE TABLE bus_type ( "
                        "type INTEGER NOT NULL CHECK (type > 0), "
                        "capacity INTEGER NOT NULL CHECK (capacity > 0), "
                       " PRIMARY KEY (type) )");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }
        ok = query.exec("CREATE TABLE salary ( "
                            "class INTEGER NOT NULL CHECK (type > 0), "
                            "exp INTEGER NOT NULL CHECK (capacity > 0), "
                            "salary INTEGER NOT NULL CHECK (salary > 0), "
                        "PRIMARY KEY (class, exp) )");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }

        ok = query.exec("CREATE TABLE driver ( "
                            "driver_id INTEGER NOT NULL AUTO_INCREMENT, "
                            "full_name VARCHAR(50) NOT NULL, "
                            "class INTEGER NOT NULL, "
                            "exp INTEGER NOT NULL, "
                        "PRIMARY KEY (driver_id), "
                        "CONSTRAINT receive FOREIGN KEY (class, exp) "
                            "REFERENCES salary (class, exp) "
                            "ON DELETE NO ACTION "
                        "ON UPDATE CASCADE)");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }

        ok = query.exec("CREATE TABLE route ( "
                            "route_id INTEGER NOT NULL, "
                            "ts TIME NOT NULL, "
                            "te TIME NOT NULL, "
                            "t INTEGER NOT NULL CHECK (exp > 0), "
                            "length DOUBLE NOT NULL CHECK (length > 0), "
                        "PRIMARY KEY (route_id))");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }

        ok = query.exec("CREATE TABLE bus ( "
                            "bus_id INTEGER NOT NULL AUTO_INCREMENT, "
                            "route_id INTEGER NOT NULL, "
                            "type INTEGER NOT NULL, "
                        "PRIMARY KEY (bus_id), "
                        "CONSTRAINT onRoute FOREIGN KEY (route_id) "
                            "REFERENCES route (route_id), "
                        "CONSTRAINT has FOREIGN KEY (type) "
                            "REFERENCES bus_type (type) "
                            "ON DELETE NO ACTION "
                            "ON UPDATE CASCADE)");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }

        ok = query.exec("CREATE TABLE control ( "
                            "driver_id INTEGER NOT NULL, "
                            "bus_id INTEGER NOT NULL, "
                        "PRIMARY KEY (driver_id, bus_id), "
                        "CONSTRAINT control FOREIGN KEY (driver_id) "
                            "REFERENCES driver (driver_id) "
                        "ON DELETE CASCADE "
                        "ON UPDATE CASCADE, "
                        "CONSTRAINT controlled FOREIGN KEY (bus_id) "
                            "REFERENCES bus (bus_id) "
                        "ON DELETE CASCADE "
                        "ON UPDATE CASCADE)");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }

        //Создание пользователей
        ok = query.exec("CREATE USER 'dispatcher'@'localhost' IDENTIFIED BY '1234'");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }
        ok = query.exec("GRANT select, insert, update, delete ON busdepot.bus TO 'dispatcher'@'localhost'");
        ok = query.exec("GRANT select, insert, update, delete ON busdepot.driver TO 'dispatcher'@'localhost'");
        ok = query.exec("GRANT select, insert, update, delete ON busdepot.control TO 'dispatcher'@'localhost'");
        ok = query.exec("GRANT select, insert, update, delete ON busdepot.route TO 'dispatcher'@'localhost'");
        ok = query.exec("GRANT select ON busdepot.salary TO 'dispatcher'@'localhost'");
        ok = query.exec("GRANT select ON busdepot.bus_type TO 'dispatcher'@'localhost'");
        if (!ok)
        {
            qDebug() << "dispatcher\n" << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }

        ok = query.exec("CREATE USER 'driver'@'localhost' IDENTIFIED BY '4321'");
        if (!ok)
        {
            qDebug() << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }

        ok = query.exec("GRANT select ON busdepot.bus TO 'driver'@'localhost'");
        ok = query.exec("GRANT select ON busdepot.driver TO 'driver'@'localhost'");
        ok = query.exec("GRANT select ON busdepot.control TO 'driver'@'localhost'");
        ok = query.exec("GRANT select ON busdepot.route TO 'driver'@'localhost'");
        ok = query.exec("GRANT select ON busdepot.salary TO 'driver'@'localhost'");
        ok = query.exec("GRANT select ON busdepot.bus_type TO 'driver'@'localhost'");
        if (!ok)
        {
            qDebug() << "driver\n" << query.lastError().text();
            QMessageBox* msg = createMessage("mainMenu", 1);
            QObject::connect(msg, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(on_exitButton_clicked()));
        }
    }
    db.close();

    ui->loginChoice->addItem("Администатор");
    ui->loginChoice->addItem("Диспетчер");
    ui->loginChoice->addItem("Водитель");
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::restore()
{
    ui->loginChoice->setCurrentIndex(0);
    ui->passwordEnter->clear();
}

void MainMenu::on_exitButton_clicked()
{
    emit exitClick();
}

void MainMenu::on_enterButton_clicked()
{
    QString password[3] = {"0000", "1234", "4321"};
    QString pswd = ui->passwordEnter->text();
    qint32 curInd = ui->loginChoice->currentIndex();
    if (pswd == password[curInd])
    {
        QSqlDatabase db = QSqlDatabase::database();
        if (curInd == 0)
        {
            db.setUserName("root");
            db.setPassword("0000");
        }
        if (curInd == 1)
        {
            db.setUserName("dispatcher");
            db.setPassword("1234");
        }
        if (curInd == 2)
        {
            db.setUserName("driver");
            db.setPassword("4321");
        }
        emit enterButtonClicked(curInd);
    }
    else
        createMessage("mainMenu", 2);
}
