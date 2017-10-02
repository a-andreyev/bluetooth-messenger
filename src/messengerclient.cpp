#include "messengerclient.h"
#include <QDebug>
#include <QBluetoothLocalDevice>
#include <QtDBus>

MessengerClient::MessengerClient(QObject *parent) : QObject(parent) {
    QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                      "net.connman.Technology", QDBusConnection::systemBus(), this);
    bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(localDevice.address());
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MessengerClient::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &MessengerClient::deviceSearchFinished);
    connect(&localDevice, &QBluetoothLocalDevice::pairingFinished,
            this, &MessengerClient::pairingFinished);
    connect(&localDevice, &QBluetoothLocalDevice::error, this, &MessengerClient::pairingError);
}

MessengerClient::~MessengerClient() {
    stopClient();
}

void MessengerClient::startDiscovery(const QString &messageToSend) {
    if (socket != NULL) stopClient();
    qDebug() << "startDiscovery()";
    this->message = messageToSend;
    discoveryAgent->start();
    emit clientStatusChanged("Searching for device");
}

void MessengerClient::stopDiscovery() {
    qDebug() << "stopDiscovery()";
    discoveryAgent->stop();
}

void MessengerClient::deviceSearchFinished() {
    qDebug() << "deviceSearchFinished()";
    if (socket == NULL) emit clientStatusChanged("Device not found");
}

void MessengerClient::deviceDiscovered(const QBluetoothDeviceInfo &deviceInfo) {
    qDebug() << "deviceDiscovered()";
    qDebug() << deviceInfo.name();
    if (deviceInfo.serviceUuids().contains(QBluetoothUuid(SERVICE_UUID))) {
        emit clientStatusChanged("Device found");
        discoveryAgent->stop();
        requestPairing(deviceInfo.address());
    }
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
    qDebug() << "requestPairing()";
    emit clientStatusChanged("Pairing devices");
    if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {
        startClient(address);
    } else {
        localDevice.requestPairing(address, QBluetoothLocalDevice::Paired);
    }
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
