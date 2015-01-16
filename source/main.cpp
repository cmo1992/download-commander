#include "mainwindow.h"
#include <QApplication>

//=============================================================================
// Method:   main(int,char*)
// Function: The main logic to start the download-commander client.
// Param 1:  int argc -
// Param 2:  char *argv[] -
// Return:   Void
//=============================================================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
