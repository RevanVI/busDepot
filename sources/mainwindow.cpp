#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tableMenu = nullptr;

    qDebug() << "mainMenu create";
    mainMenu = new MainMenu();
    QObject::connect(mainMenu, SIGNAL(enterButtonClicked(int)), this, SLOT(enterTableMenu(int)));
    QObject::connect(mainMenu, SIGNAL(exitClick()), this, SLOT(exitClick()));
    ui->layt->addWidget(mainMenu);
    this->setWindowTitle("BusDepot");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enterTableMenu(int rang)
{
    tableMenu = new TableMenu(rang);
    QObject::connect(tableMenu, SIGNAL(backButtonClick()), this, SLOT(returnMainMenu()));
    mainMenu->hide();
    ui->layt->addWidget(tableMenu);
}

void MainWindow::returnMainMenu()
{
    //Восстановление исходного состояния mainMenu
    mainMenu->restore();
    tableMenu->hide();
    mainMenu->show();
    delete tableMenu;
    tableMenu = nullptr;
}

void MainWindow::exitClick()
{
    emit exitButtonClicked();
}
