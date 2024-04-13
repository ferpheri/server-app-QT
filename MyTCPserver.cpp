#include "MyTCPserver.h"

MyTCPserver::MyTCPserver(QString ip ,int port, int period , QObject *parent)
    : QObject{parent}, _server{nullptr}, timer{nullptr}, _isStarted{false}
{
    _period = period;
    _server = new QTcpServer(this);
    if (!_server->listen(QHostAddress(ip), port)) {
        qDebug() << "Server could not start";
    } else {
        qDebug() << "Server started...";
        connect(_server, &QTcpServer::newConnection, this, &MyTCPserver::on_client_connecting);
        _isStarted = true;
    }
}

void MyTCPserver::on_client_connecting()
{
    auto socket = _server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &MyTCPserver::clientDataReady);
    connect(socket, &QTcpSocket::disconnected, this, &MyTCPserver::clientDisconnected);
    _socketList.append(socket);
    emit newClientConnected();
}

void MyTCPserver::clientDataReady()
{
    auto socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
        return;

    auto data = socket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    if (message.startsWith("Sensor")) {
        handleSensorSelection(message);
    } else {
        emit dataReceived(message);
    }
}

void MyTCPserver::clientDisconnected()
{
    emit clientDisconnect();
}

void MyTCPserver::handleSensorSelection(QString sensor)
{
    QString sensorNumber = sensor.mid(6);
    QString requestMessage = QString("Client requested for sensor %1").arg(sensorNumber);
    emit dataReceived(requestMessage);
    stopTimer();

    timer = new QTimer(this);
    if (!timer) {
        qDebug() << "Error: Failed to create timer.";
        return;
    }

    connect(timer, &QTimer::timeout, this, [=]() {
        QString currentTime = QTime::currentTime().toString("hh:mm:ss");
        QByteArray responseData;

        if (sensor == "Sensor1") {
            responseData = QString("Sensor1 -> Time: %1 Data:11111").arg(currentTime).toUtf8();
        } else if (sensor == "Sensor2") {
            responseData = QString("Sensor2 -> Time: %1 Data:22222").arg(currentTime).toUtf8();
        } else if (sensor == "Sensor3") {
            responseData = QString("Sensor3 -> Time: %1 Data:33333").arg(currentTime).toUtf8();
        }

        sendToAll(responseData);
    });
    timer->start(_period);
}

void MyTCPserver::stopTimer()
{
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
}

void MyTCPserver::sendToAll(const QByteArray &message)
{
    for (QTcpSocket *socket : _socketList) {
        if (socket && socket->state() == QAbstractSocket::ConnectedState)
            socket->write(message);
    }
}

void MyTCPserver::stopServer()
{
    stopTimer();
    for (QTcpSocket *socket : _socketList) {
        if (socket) {
            socket->disconnectFromHost();
            if (socket->state() != QAbstractSocket::UnconnectedState)
                socket->waitForDisconnected(2000);
        }
    }
    _socketList.clear();
    _server->close();
}

bool MyTCPserver::isStarted() const
{
    return _isStarted;
}
