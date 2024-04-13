#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _server = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartServer_clicked()
{
    if(_server == nullptr){
        auto port = ui->spnServerPort->value();
        auto ip = ui->InIPAddress->text();
        auto period = ui->spnPeriod->value();
        _server = new MyTCPserver(ip,port,period);
        connect(_server, &MyTCPserver::newClientConnected,this, &MainWindow::newClient_connected);
        connect(_server, &MyTCPserver::dataReceived,this, &MainWindow::clientDataReceived);
        connect(_server, &MyTCPserver::clientDisconnect,this, &MainWindow::clientDisconnected);
        if(_server->isStarted()){
            ui->btnStartServer->setText("Stop Server");
            ui->lblConnectionStatus->setProperty("state","0");
            style()->polish(ui->lblConnectionStatus);
        }else{
            qDebug() << "Failed to start the server.";
        }

    }else{
        _server->stopServer();
        // delete _server;
        _server = nullptr;
        ui->btnStartServer->setText("Start Server");
        ui->lblConnectionStatus->setProperty("state","1");
        style()->polish(ui->lblConnectionStatus);
    }
}

void MainWindow::newClient_connected()
{
    ui->lstConsole->addItem("New Client connected");
}

void MainWindow::clientDisconnected()
{
    ui->lstConsole->addItem("Client Disconnected");
}

void MainWindow::clientDataReceived(QString message)
{
    ui->lstConsole->addItem(message);
}


void MainWindow::on_InIPAddress_textChanged(const QString &arg1)
{
    QString state = "0";
    if(arg1 == "..."){
        state = "";
    }else{
        QHostAddress address(arg1);
        if(QAbstractSocket::IPv4Protocol == address.protocol()){
            state = "1";
        }
    }
    ui->InIPAddress->setProperty("state",state);
    style()->polish(ui->InIPAddress);
}

