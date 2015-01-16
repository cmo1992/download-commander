#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStringList>
#include <QListWidgetItem>

//=============================================================================
// Method:   MainWindow(QWidget*)
// Function: Constructor
// Param 1:  QObject* parent -  Sets the parent.
// Return:   Void
//=============================================================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port = 1234;
    call = new MyClient(this);
    connect(call, SIGNAL(receiverFileList(QStringList)), this, SLOT(receiveList(QStringList)));
    connect(call, SIGNAL(connection(bool)), this, SLOT(setConnectionStatus(bool)));
    connect(this, SIGNAL(getConntecionStatus()), call, SLOT(isConnected()));
    emit getConntecionStatus();
}

//=============================================================================
// Method:   ~MainWindow()
// Function: Deconstructor
// Return:   Void
//=============================================================================
MainWindow::~MainWindow()
{
    delete call;
    delete ui;
}


///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS...
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// PUBLIC SLOTS...
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Method:   on_btnConnect_clicked()
// Function: Connects and disconnects to the server.
// Param:    None
// Return:   Void
//=============================================================================
void MainWindow::on_btnConnect_clicked()
{
    if((ui->txtIP->text()).length() > 0){
        if(ui->lblConnected->text() == "No Connection"){
            ipAddress = ui->txtIP->text();
            call->start(ipAddress, port);
        } else {
            call->end();
        }
        emit getConntecionStatus();
    } else {

    }
}

//=============================================================================
// Method:   on_btnFind_clicked()
// Function: Opens up a dialog box to select the file that is going to be
//           transfered.
// Param:    None
// Return:   Void
//=============================================================================
void MainWindow::on_btnFind_clicked()
{
    ui->txtfilepath->setText(QFileDialog::getOpenFileName
                             (this,"Select File To Transfer","/home/cmo"));
}

//=============================================================================
// Method:   on_btnAdd_clicked()
// Function: Calls the client's "sendFileToServer" function.
// Param:    None
// Return:   Void
//=============================================================================
void MainWindow::on_btnAdd_clicked()
{
    ui->txtLog->append("Starting to upload file...\n");
    QString msg = call->sendFileToServer(ui->txtfilepath->text(),
                                         ui->txtLink->text());
    ui->txtLog->append(msg);
    ui->txtfilepath->setText("");
    ui->txtLink->setText("");
}

//=============================================================================
// Method:   on_tabWidget_currentChanged(int)
// Function: Check when the tab is changed to resond correctly. When moving to
//           the second tab a list update function is called.
// Param:    int index - The tab number.
// Return:   Void
//=============================================================================
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 0){

    }else if (index == 1){
        requestFileLst();
    }
}

//=============================================================================
// Method:   receiveList(QString)
// Function: Receives the list of strings and updates the list widget.
// Param:    QString ls
// Return:   Void
//=============================================================================
void MainWindow::receiveList(QStringList ls){
    ui->txtLog->append("=================================\n");
    ui->txtLog->append(ls.join("\n"));
    ui->txtLog->append("=================================\n");
    ui->listWidget->clear();
    ui->listWidget->addItems(ls);
}

//=============================================================================
// Method:   on_btnClose_clicked()
// Function: Closes the Application when the "Close button is clicked.
// Param:    None
// Return:   Void
//=============================================================================
void MainWindow::on_btnClose_clicked()
{
    QApplication::quit();
}

//=============================================================================
// Method:   setConnectionStatus(bool)
// Function: Makes changes to the interface when there is a connection and
//           when there is no connection with a server.
// Param:    bool conStat - the status of the connection.
// Return:   Void
//=============================================================================
void MainWindow::setConnectionStatus(bool conStat){
    if(conStat){
        ui->lblConnected->setText("Connected");
        ui->btnConnect->setText("Disconnect");
        ui->btnAdd->setEnabled(true);
        ui->btnRem->setEnabled(true);
        ui->tabRem->setEnabled(true);
        ui->txtLog->append("Connected to IP: " + ipAddress + " Port: " +
                           QString::number(port));
    } else {
        ui->lblConnected->setText("No Connection");
        ui->btnConnect->setText("Connect");
        ui->btnAdd->setEnabled(false);
        ui->btnRem->setEnabled(false);
        ui->tabRem->setEnabled(false);
        ui->txtLog->append("Closed connection to IP: " + ipAddress);
        ui->txtLog->append("=================================\n\n");
    }
}

///////////////////////////////////////////////////////////////////////////////
// SUB PRIVATE METHODS...
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Method:   on_btnRem_clicked()
// Function: The function is called when the "Remove" button is clicked and
//           then calls the clients requestFileRemovel for all the selected
//           items. Then calls
// Param:    None
// Return:   Void
//=============================================================================
void MainWindow::on_btnRem_clicked()\
{
    ui->txtLog->append("Starting to remove file...");
    QList<QListWidgetItem*> wIfiles = ui->listWidget->selectedItems();
    QListIterator<QListWidgetItem*> itr(wIfiles);
    while(itr.hasNext()){
        QListWidgetItem *item = itr.next();
        QString msg = call->requestFileRemovel(item->text());
        ui->txtLog->append(msg);
    }
    requestFileLst();
}

///////////////////////////////////////////////////////////////////////////////
// PROTECTED...
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// MAIN PRIVATE METHODS...
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Method:   requestFileLst()
// Function: The function is called when the "List" needs to be updated. Calls
//           the client's 'requestFileList() function then writes the message
//           to the log.
// Param:    None
// Return:   Void
//=============================================================================
void MainWindow::requestFileLst(){
    QString msg = call->requestFileList();
    ui->txtLog->append(msg);
}

///////////////////////////////////////////////////////////////////////////////
// SUB PRIVATE METHODS...
///////////////////////////////////////////////////////////////////////////////
