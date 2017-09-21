#include "messengerclient.h"
#include <QDebug>
#include <QBluetoothLocalDevice>

MessengerClient::MessengerClient(QObject *parent) : QObject(parent) {
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(QBluetoothLocalDevice().address());
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceSearchFinished()));
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
        startClient(deviceInfo);
    }
}

void MessengerClient::startClient(QBluetoothDeviceInfo deviceInfo) {
    qDebug() << "startClient()";
    if (socket != NULL) {
        socket->disconnectFromService();
        delete socket;
    }
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    socket->connectToService(deviceInfo.address(), 1);
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
