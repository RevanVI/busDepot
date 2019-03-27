#ifndef MAINMENU_H
#define MAINMENU_H

#include <QtWidgets>
#include <QtSql>
#include "createmessage.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    void restore();
    ~MainMenu();

private slots:
    void on_exitButton_clicked();
    void on_enterButton_clicked();

private:
    Ui::MainMenu *ui;

signals:
    void exitClick();
    void enterButtonClicked(int rang);
};

#endif // MAINMENU_H
