#include "jcliehandler.h"
#include <QProcess>
#include <QDebug>
//=============================================================================
// Method:   JClieHandler(QString, QString, QObject*)
// Function: Constructor
// Param 1:  QString ipAddress - The IP Address of the server
// Param 2:  QString path - The path the the file on the system
// Param 3:  QObject* parent - The parent class
// Return:   Void
//=============================================================================
JClieHandler::JClieHandler(QString ipAddress, QString path, QObject* parent)  : QThread(parent) {
    ipAdd = ipAddress;
    qDebug() << "Thread pram 1: " << ipAddress;
    fLoc = path;
    qDebug() << "Thread pram 2: " << path;
}

//=============================================================================
// Method:   ~JClieHandler()
// Function: Deconstructor
// Param:    None
// Return:   Void
//=============================================================================
JClieHandler::~JClieHandler()
{

}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS...
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// PUBLIC SLOTS...
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Method:   run()
// Function: Runs the jClie.jar file with the IP Address and file path as
//           arguments. Then emits the log messages from the jClie.jar.
// Param:    None
// Return:   Void
//=============================================================================
void JClieHandler::run(){
    qDebug() << "Thread has started...";
    QProcess run;
    qDebug() << "Thread run object created...";
    run.execute("java -jar jClie.jar \"" + ipAdd + "\" \"" + fLoc +"\"");
    qDebug() << "Thread executed java -jar jClie.jar \"" << ipAdd << "\" \"" << fLoc << "\"";
    run.waitForFinished();
    msg.append(QString(run.readAll()));
    qDebug() << QString(run.readAll());
    msg.append("File Sent!");
    qDebug() << "Thread has ended.";
    emit logReady(msg);
}

///////////////////////////////////////////////////////////////////////////////
// PROTECTED...
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// MAIN PRIVATE METHODS...
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// SUB PRIVATE METHODS...
///////////////////////////////////////////////////////////////////////////////
