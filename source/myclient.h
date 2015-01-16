#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QTcpSocket>

class MyClient: public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QObject* parent = 0);
    ~MyClient();
    void start(QString address, quint16 port);
    void end();
    QString sendFileToServer(QString fn, QString inLink);
    QString requestFileList();
    QString requestFileRemovel(QString fn);

public slots:
    void startTransfer();
    void receiverLog(QString result);
    void isConnected();

signals:
    void receiverFileList(QStringList ls);
    void connection(bool con);

private:
    //main functions
    void sendFile();// /*, QDataStream *out*/);
    void removeFile();
    void getList();
    //sub functions
    void writeToServer(QString str);
    QString getFileName(QString fileName);
    void delay( int millisecondsToWait );
    void resetTransfer();



    //variables
    bool needToSendFile, needToRemFile, needToGetList;
    QString fileName, link, msg, ipAddress;
    QTcpSocket client;
};


#endif // MYCLIENT_H
