#include "messengerclient.h"
#include <QDebug>
#include <QBluetoothLocalDevice>
#include <QtDBus>

MessengerClient::MessengerClient(QObject *parent) : QObject(parent) {
    QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                      "net.connman.Technology", QDBusConnection::systemBus(), this);
    bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(localDevice.address());
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceSearchFinished()));
    connect(&localDevice, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)),
            this, SLOT(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));
    connect(&localDevice, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(pairingError(QBluetoothLocalDevice::Error)));
}

MessengerClient::~MessengerClient() {
    stopClient();
}

void MessengerClient::startDiscovery(QString messageToSend) {
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

void MessengerClient::deviceDiscovered(QBluetoothDeviceInfo deviceInfo) {
    qDebug() << "deviceDiscovered()";
    qDebug() << deviceInfo.name();
    if (deviceInfo.serviceUuids().toSet().contains(QBluetoothUuid(SERVICE_UUID))) {
        emit clientStatusChanged("Device found");
        discoveryAgent->stop();
        requestPairing(deviceInfo.address());
    }
}

void MessengerClient::pairingFinished(QBluetoothAddress address,
                                      QBluetoothLocalDevice::Pairing pairing) {
    qDebug() << "pairingFinished()";
    startClient(address);
}

void MessengerClient::pairingError(QBluetoothLocalDevice::Error error) {
    qDebug() << "pairingError()";
    emit clientStatusChanged("Unable to pair devices");
}

void MessengerClient::requestPairing(QBluetoothAddress address) {
    qDebug() << "requestPairing()";
    emit clientStatusChanged("Pairing devices");
    qDebug() << address.toString();
    if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {
        startClient(address);
    } else {
        localDevice.requestPairing(address, QBluetoothLocalDevice::Paired);
    }

}

void MessengerClient::startClient(QBluetoothAddress address) {
    qDebug() << "startClient()";
    if (socket != NULL) {
        socket->disconnectFromService();
        delete socket;
    }
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
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
    QString receivedMessage = QString::fromUtf8(socket->readLine().trimmed().constData());
    emit messageReceived(receivedMessage);
    emit clientStatusChanged("Message received");
    stopClient();
}
