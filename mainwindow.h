#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyle>
#include "MyTCPserver.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStartServer_clicked();
    void newClient_connected();
    void clientDisconnected();
    void clientDataReceived(QString message);
    void on_InIPAddress_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    MyTCPserver *_server;
};
#endif // MAINWINDOW_H
