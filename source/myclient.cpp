#include "myclient.h"
#include "jcliehandler.h"
#include <QHostAddress>

//=============================================================================
// Method:   MyClient(QObject*)
// Function: Constructor
// Param 1:  QObject* parent -  Sets the parent.
// Return:   Void
//=============================================================================

MyClient::MyClient(QObject* parent): QObject(parent)
{
    resetTransfer();
    connect(&client, SIGNAL(connected()), this, SLOT(startTransfer()));
    //    connect(&client, SIGNAL(disconnected()), this, SLOT(disconnected());
}

//=============================================================================
// Method:   ~MyClient()
// Function: Closes the connection with the server.
// Param:    None
// Return:   Void
//=============================================================================
MyClient::~MyClient() {
    client.close();
}

//=============================================================================
// Method:   end()
// Function: Disconnects from the server.
// Param:    None
// Return:   Void
//=============================================================================
void MyClient::end(){
    client.close();
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS...
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Method:   start(QString, quint16)
// Function: Connects the client to the server.
// Param 1:  QString address - The servers IP address.
// Param 2:  quint16 port - The port the server is listening on.
// Return:   Viod
//=============================================================================
void MyClient::start(QString address, quint16 port) {
    ipAddress = address;
    QHostAddress addr(address);
    client.connectToHost(addr, port);
}

//=============================================================================
// Method:   sendFileToServer(QString, QString)
// Function: Sets the transfer variables, and calls 'startTransfer().'
// Param 1:  QString inPath - The path to the file that will be transmited.
// Param 1:  QString inLink - The title of the link for the web page.
// Return:   QString rMsg - The messages that are send to the calling class.
//=============================================================================
QString MyClient::sendFileToServer(QString fn, QString inLink){
    needToSendFile = true;
    needToRemFile = false;
    needToGetList = false;
    fileName = fn;
    link = inLink;

    startTransfer();//TODO: adding msg to send back to gui log
    QString rMsg = msg;
    msg = "";
    return rMsg;
}

//=============================================================================
// Method:   requestFileList(QStringList)
// Function: Sets the transfer variables, and calls 'startTransfer().'
// Return:   QString rMsg - The messages that are send to the calling class.
//=============================================================================
QString MyClient::requestFileList(){
    needToSendFile = false;
    needToRemFile = false;
    needToGetList = true;

    startTransfer();//TODO: adding msg to send back to gui log
    QString rMsg = msg;
    msg = "";
    return rMsg;
}

//=============================================================================
// Method:   requestFileRemovel(QString)
// Function: Sets the transfer variables, and calls 'startTransfer().'
// Param 1:  QString inPath - The path to the file that will be transmited.
// Return:   QString rMsg - The messages that are send to the calling class.
//=============================================================================
QString MyClient::requestFileRemovel(QString fn){
    needToSendFile = false;
    needToRemFile = true;
    needToGetList = false;
    fileName = fn;
    startTransfer();//TODO: adding msg to send back to gui log
    QString rMsg = msg;
    msg = "";
    return rMsg;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC SLOTS...
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Method:   startTransfer()
// Function: Selects the function needed from server.
// Param:    None
// Return:   Void
//=============================================================================
void MyClient::startTransfer()
{
    if(needToSendFile == true && needToRemFile == false &&
            needToGetList == false) {
        writeToServer("ADD");
        sendFile();
    } else if(needToSendFile == false && needToRemFile == true &&
              needToGetList == false) {
        writeToServer("REM");
        removeFile();
    } else if(needToSendFile == false && needToRemFile == false &&
              needToGetList == true) {
        writeToServer("LST");
        getList();
    } else {
        qDebug() << "Nothing to transmit...";
    }
    resetTransfer();
}

//=============================================================================
// Method:   receiverLog(QString)
// Function: Receives the log from the jClie thread.
// Param:    QString result - The log message from jClie thread.
// Return:   Void
//=============================================================================
void MyClient::receiverLog(QString result){
    msg.append(result);
}

//=============================================================================
// Method:   isConnected()
// Function: Transmits the sockets connection status.
// Param:    None
// Return:   Void
//=============================================================================
void MyClient::isConnected(){
    emit connection(client.isOpen());
}

///////////////////////////////////////////////////////////////////////////////
// MAIN PRIVATE METHODS...
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Method:   sendFile(QString, QString)
// Function: Sends the fileName and link to the server. Then calls the
//              jClie.jar to send the file to the server.
// Param 1:  QString inPath - The path to the file that will be transmited.
// Param 1:  QString inLink - The title of the link for the web page.
// Return:   Void
//=============================================================================
void MyClient::sendFile() {
    //Opening the file...
    QString filename = getFileName(fileName);
    writeToServer(filename + '\n');
    msg.append("Sending File Name: " + filename + '\n');
    writeToServer(link  + '\n');
    msg.append("Sending File Link: " + link + '\n');
    msg.append("Sending File Data...\n");
    qDebug() << ipAddress << filename;

    JClieHandler *jClie = new JClieHandler(ipAddress, fileName, this);
    connect(jClie, SIGNAL(logReady(QString)), this, SLOT(receiverLog(QString)));
    connect(jClie, SIGNAL(finished()), jClie, SLOT(deleteLater()));
    jClie->start();
//    QProcess run;
//    run.execute("java -jar jClie.jar \"" + ipAddress + "\" \"" + path +"\"");
//    run.waitForFinished();
//    msg.append(QString(run.readAll()));
    QString tmp;
    msg.append("Waiting...\n");
    client.waitForReadyRead();
    msg.append("receiving...\n");
    tmp = client.readLine();
    if(tmp == "ADD_PASS"){
        msg.append("File Successfully Sent.\n");
    } else {
        msg.append("Failed! To Send File...\n");
    }
}

//=============================================================================
// Method:   removeFile()
// Function: Sends the fileName to the server. Then waits for the response.
// Return:   Void
//=============================================================================
void MyClient::removeFile(){
    msg.append("Sending file to remove...\n");
    qDebug() << "!!!!!!!" << fileName;
    writeToServer(fileName + '\n');
    QString tmp;
    client.waitForReadyRead();
    tmp = client.readLine();
    if(tmp == "REM_PASS"){
        msg.append("File Successfully Removed.\n");
    } else {
        msg.append("Failed! No file to remove...\n");
    }
}

//=============================================================================
// Method:   getList()
// Function: Waits for server to send list.
// Return:   Void
//=============================================================================
void MyClient::getList(){
    msg.append("Waiting for list...\n");
    QStringList lis;
    QString tp;
    delay(100);
    tp = client.readLine();
    tp = tp.trimmed();
    qDebug() << tp;
    int i = 0;
    int max = tp.toInt();
    while(i < max){
        QString tmp;
        tmp = client.readLine();
        tmp = tmp.trimmed();
        qDebug() << tmp;
        msg.append("File " + QString::number(i + 1) + ": " + tmp + '\n');
        lis.append(tmp);
        i++;
    }
    emit receiverFileList(lis);
}

///////////////////////////////////////////////////////////////////////////////
// SUB PRIVATE METHODS...
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// Method:   writeToServer(QString)
// Function: Sends the message to the server.
// Param:    QString str - The message to be send.
// Return:   Void
//=============================================================================
void MyClient::writeToServer(QString str)
{
    client.write(str.toStdString().data());
    client.flush();
    client.waitForBytesWritten();
}

//=============================================================================
// Method:   getFileName(QString)
// Function: Pulls the file name out of the path.
// Param:    QString path - The location of the file.
// Return:   QString fileName - The name of the file.
//=============================================================================
QString MyClient::getFileName(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        msg.append("Opened file: " + file.fileName() + "\n");
    }
    QFileInfo fileInfo(file.fileName());
    QString fileName(fileInfo.fileName());
    file.close();

    return fileName;
}

//=============================================================================
// Method:   delay(int)
// Function: Pauses the exicution for a period of time
// Param:    int millisecondsToWait - The time that is wanted to pause.
// Return:   Void
//=============================================================================
void MyClient::delay( int millisecondsToWait ){
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

//=============================================================================
// Method:   resetTransfer()
// Function: Sets all the transfer variables to false.
// Param:    None
// Return:   Void
//=============================================================================
void MyClient::resetTransfer()
{
    needToSendFile = false;
    needToRemFile = false;
    needToGetList = false;
}

