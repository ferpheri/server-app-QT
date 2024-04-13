#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

class MyTCPserver : public QObject
{
    Q_OBJECT
public:
    explicit MyTCPserver(QString ip,int port , int period, QObject *parent = nullptr);
    bool isStarted() const;
    void sendToAll(const QByteArray &message);
    void stopServer();
    int _period;

signals:
    void newClientConnected();
    void clientDisconnect();
    void dataReceived(QString message);

private slots:
    void on_client_connecting();
    void clientDataReady();
    void clientDisconnected();
    void handleSensorSelection(QString sensor);
    void stopTimer();

private:
    QTcpServer *_server;
    QList<QTcpSocket *> _socketList;
    QTimer* timer;
    bool _isStarted;

};

#endif // MYTCPSERVER_H
