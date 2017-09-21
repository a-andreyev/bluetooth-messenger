#ifndef MESSENGERCLIENT_H
#define MESSENGERCLIENT_H

#include <QObject>

#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothSocket>

class MessengerClient : public QObject {
    Q_OBJECT
public:
    explicit MessengerClient(QObject *parent = 0);
    ~MessengerClient();
    Q_INVOKABLE void startDiscovery(QString messageToSend);
    Q_INVOKABLE void stopDiscovery();
private:
    const QString SERVICE_UUID = "1f2d6c5b-6a86-4b30-8b4e-3990043d73f1";
    QString message;
    QBluetoothSocket *socket = NULL;
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothDeviceInfo device;
    void startClient(QBluetoothDeviceInfo deviceInfo);
    void stopClient();
signals:
    void messageReceived(QString message);
    void clientStatusChanged(QString text);
private slots:
    void deviceDiscovered(QBluetoothDeviceInfo deviceInfo);
    void socketConnected();
    void deviceSearchFinished();
    void readSocket();
};

#endif // MESSENGERCLIENT_H
