#ifndef MESSENGERSERVER_H
#define MESSENGERSERVER_H

#include <QObject>
#include <QBluetoothServer>

class MessengerServer : public QObject {
    Q_OBJECT
public:
    explicit MessengerServer(QObject *parent = 0);
    ~MessengerServer();
    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();

signals:
    void messageReceived(QString message);
private:
    QBluetoothServer *bluetoothServer;
    QBluetoothServiceInfo serviceInfo;
    QBluetoothSocket *socket;
    const QString SERVICE_UUID = "1f2d6c5b-6a86-4b30-8b4e-3990043d73f1";
private slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();
};

#endif // MESSENGERSERVER_H
