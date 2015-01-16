#ifndef JCLIEHANDLER_H
#define JCLIEHANDLER_H

#include <QObject>
#include <QThread>

class JClieHandler : public QThread
{
    Q_OBJECT
public:

    explicit JClieHandler(QString ipAddress, QString path,QObject* parent = 0);
    ~JClieHandler();
    void run();

signals:
    void logReady(QString result);

private:
    QString ipAdd, fLoc, msg;


};

#endif // JCLIEHANDLER_H
