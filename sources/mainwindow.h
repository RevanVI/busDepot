#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "mainmenu.h"
#include "tablemenu.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void enterTableMenu(int);
    void returnMainMenu();
    void exitClick();

signals:
    void exitButtonClicked();
private:
    Ui::MainWindow *ui;
    MainMenu* mainMenu;
    TableMenu* tableMenu;


};

#endif // MAINWINDOW_H
