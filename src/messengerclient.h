#ifndef MESSENGERCLIENT_H
#define MESSENGERCLIENT_H

#include <QObject>

#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>

class MessengerClient : public QObject {
    Q_OBJECT
public:
    explicit MessengerClient(QObject *parent = 0);
    ~MessengerClient();
    Q_INVOKABLE void startDiscovery(const QString &messageToSend);
    Q_INVOKABLE void stopDiscovery();
private:
    const QString SERVICE_UUID = "1f2d6c5b-6a86-4b30-8b4e-3990043d73f1";
    QString message;
    QBluetoothSocket *socket = NULL;
    QBluetoothServiceDiscoveryAgent *discoveryAgent;
    // QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothDeviceInfo device;
    QBluetoothLocalDevice localDevice;
    void requestPairing(const QBluetoothAddress &address);
    void startClient(const QBluetoothAddress &address);
    void stopClient();
signals:
    void messageReceived(QString message);
    void clientStatusChanged(QString text);
private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);
    void pairingError(QBluetoothLocalDevice::Error error);
    void socketConnected();
    void readSocket();
};

#endif // MESSENGERCLIENT_H
