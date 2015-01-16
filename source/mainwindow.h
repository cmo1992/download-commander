#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnFind_clicked();

    void on_btnConnect_clicked();

    void on_btnAdd_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_btnClose_clicked();

    void on_btnRem_clicked();

    void receiveList(QStringList ls);

    void setConnectionStatus(bool conStat);

signals:
    void getConntecionStatus();

private:
    void requestFileLst();

    Ui::MainWindow *ui;
    MyClient *call;
    QString ipAddress;
    qint64 port;
};

#endif // MAINWINDOW_H
