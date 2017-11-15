#include "messengerclient.h"
#include <QDebug>
#include <QBluetoothLocalDevice>
#include <QtDBus>

MessengerClient::MessengerClient(QObject *parent) : QObject(parent) {
    /*
    QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                      "net.connman.Technology", QDBusConnection::systemBus(), this);
    bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));
    */

    /*
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(localDevice.address());
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MessengerClient::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &MessengerClient::deviceSearchFinished);
    connect(&localDevice, &QBluetoothLocalDevice::pairingFinished,
            this, &MessengerClient::pairingFinished);
    connect(&localDevice, &QBluetoothLocalDevice::error, this, &MessengerClient::pairingError);
    */

    // scan for services
    discoveryAgent = new QBluetoothServiceDiscoveryAgent();

    connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
    connect(discoveryAgent, SIGNAL(canceled()), this, SLOT(discoveryFinished()));
}

MessengerClient::~MessengerClient() {
    stopClient();
}

void MessengerClient::startDiscovery(const QString &messageToSend) {
    if (socket != NULL) stopClient();
    qDebug() << Q_FUNC_INFO;
    this->message = messageToSend;
    discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
    emit clientStatusChanged("Searching for service");
}

void MessengerClient::stopDiscovery() {
    qDebug() << Q_FUNC_INFO;
    discoveryAgent->stop();
}

void MessengerClient::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();
    qDebug() << "\tService UUID:" << serviceInfo.serviceUuid().toString();
    if (serviceInfo.serviceUuid().toString().contains(SERVICE_UUID)) {
        qDebug() << "This is our service";
        emit clientStatusChanged("Device found");
        discoveryAgent->stop();
        requestPairing(serviceInfo.device().address());
    }

}

void MessengerClient::discoveryFinished()
{
    emit clientStatusChanged("Discovery stopped");
    qDebug()<<Q_FUNC_INFO;
}

void MessengerClient::pairingFinished(const QBluetoothAddress &address,
                                      QBluetoothLocalDevice::Pairing pairing) {
    qDebug() << "pairingFinished()";
    startClient(address);
}

void MessengerClient::pairingError(QBluetoothLocalDevice::Error error) {
    qDebug() << "pairingError()";
    emit clientStatusChanged("Unable to pair devices");
}

void MessengerClient::requestPairing(const QBluetoothAddress &address) {
    emit clientStatusChanged("Pairing devices");
    qDebug() << Q_FUNC_INFO << localDevice.pairingStatus(address);
    startClient(address);
    /*
    if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {
        startClient(address);
    } else {
        localDevice.requestPairing(address, QBluetoothLocalDevice::Paired);
    }
    */
}

void MessengerClient::startClient(const QBluetoothAddress &address) {
    qDebug() << "startClient()";
    if (socket != NULL) {
        socket->disconnectFromService();
        delete socket;
    }
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(socket, &QBluetoothSocket::connected, this, &MessengerClient::socketConnected);
    connect(socket, &QBluetoothSocket::readyRead, this, &MessengerClient::readSocket);
    socket->connectToService(address, 1);
}

void MessengerClient::socketConnected() {
    qDebug() << "socketConnected()";
    emit clientStatusChanged("Connected to socket");
    socket->write(message.toUtf8());
    emit clientStatusChanged("Message sent");
}

void MessengerClient::stopClient() {
    qDebug() << "stopClient()";
    if (socket != NULL) {
        socket->disconnectFromService();
        delete socket;
    }
    socket = NULL;
}

void MessengerClient::readSocket() {
    qDebug() << "readSocket()";
    QString receivedMessage = QString::fromUtf8(socket->readLine().trimmed());
    emit messageReceived(receivedMessage);
    emit clientStatusChanged("Message received");
    stopClient();
}
