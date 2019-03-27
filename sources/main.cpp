#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    MainWindow w;
    w.show();
    QObject::connect(&w, SIGNAL(exitButtonClicked()), &a, SLOT(quit()));
    return a.exec();
}
