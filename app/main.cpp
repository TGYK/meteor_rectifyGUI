//============================================================================
// Name        : main.cpp
// Author      : TGYK
// Date        : 12/14/2020
// E-Mail      : tgyk@tgyk.net
// Description : This is where the application starts from..
//               We all gotta start somewhere.
//============================================================================

#include <iostream>
#include <QApplication>
#include "mainwindow.h"

using namespace std;

QMutex RectifyThread::mutex;

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
