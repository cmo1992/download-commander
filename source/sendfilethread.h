#ifndef SENDFILETHREAD_H
#define SENDFILETHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

class SendFileThread : public QThread
{
    Q_OBJECT
public:
    explicit SendFileThread(int ID, QObject *parent = 0);
    void run();
    ~SendFileThread();

signals:

public slots:
    void sockerRecieve(QTcpSocket *c);

private:
    int socketDescriptor;
    QTcpSocket *client;
    void sendFile(QString path);
};

#endif // SENDFILETHREAD_H
