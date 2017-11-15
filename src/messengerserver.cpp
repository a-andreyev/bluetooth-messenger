#include "messengerserver.h"
#include <QBluetoothLocalDevice>
#include <QtDBus>

MessengerServer::MessengerServer(QObject *parent) : QObject(parent) {
    // turning on bt:
    /*
    QDBusInterface bluetoothInterface("net.connman", "/net/connman/technology/bluetooth",
                                      "net.connman.Technology", QDBusConnection::systemBus(), this);
    bluetoothInterface.call("SetProperty", "Powered", QVariant::fromValue(QDBusVariant(true)));
    */

    // setting default adapter to discoverable:

    // adapter_path = bluezutils.find_adapter(options.dev_id).object_path
    // adapter = dbus.Interface(bus.get_object("org.bluez", adapter_path),
    //                    "org.freedesktop.DBus.Properties")

    /*
    QDBusInterface adapterListInterface("org.bluez", "/", "org.bluez.Manager",
                                        QDBusConnection::systemBus(), this);
    QVariant adapterPath = adapterListInterface.call("DefaultAdapter").arguments().at(0);
    QDBusInterface bluetoothAdapter("org.bluez", adapterPath.value<QDBusObjectPath>().path(),
                                    "org.bluez.Adapter", QDBusConnection::systemBus(), this);
    bluetoothAdapter.call("SetProperty", "DiscoverableTimeout", QVariant::fromValue(QDBusVariant(0U)));
    bluetoothAdapter.call("SetProperty", "Discoverable", QVariant::fromValue(QDBusVariant(true)));
    */
}

MessengerServer::~MessengerServer() {
    stopServer();
}

void MessengerServer::startServer() {
    qDebug() << "startServer()";
    if (bluetoothServer) return;
    bluetoothServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(bluetoothServer, &QBluetoothServer::newConnection,
            this, &MessengerServer::clientConnected);
    QBluetoothAddress bluetoothAddress = QBluetoothLocalDevice().address();
    bool result = bluetoothServer->listen(bluetoothAddress);
    if (!result) {
        qWarning() << "Cannot bind chat server to" << bluetoothAddress.toString();
        return;
    }
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);
    classId.prepend(QVariant::fromValue(QBluetoothUuid(SERVICE_UUID)));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, "BT message sender");
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             "Example message sender");
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, "fruct.org");
    serviceInfo.setServiceUuid(QBluetoothUuid(SERVICE_UUID));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);

    QBluetoothServiceInfo::Sequence protocol;
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(bluetoothServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocolDescriptorList);

    qDebug()<<serviceInfo.registerService(bluetoothAddress);
}

void MessengerServer::stopServer() {
    qDebug() << "stopServer()";
    if (serviceInfo.isRegistered()) serviceInfo.unregisterService();
    if (bluetoothServer != NULL) delete bluetoothServer;
    bluetoothServer = NULL;
}

void MessengerServer::clientConnected() {
    qDebug() << "clientConnected()";
    socket = bluetoothServer->nextPendingConnection();
    connect(socket, &QBluetoothSocket::readyRead, this, &MessengerServer::readSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, &MessengerServer::clientDisconnected);
}

void MessengerServer::clientDisconnected() {
    qDebug() << "clientDisconnected()";
    socket->deleteLater();
    socket = NULL;
}

void MessengerServer::readSocket() {
    qDebug() << "readSocket()";
    const QString message = QString::fromUtf8(socket->readLine().trimmed());
    emit messageReceived(message);
    QString reversedMessage;
    for (int i = message.size() - 1; i >= 0; i--) {
        reversedMessage.append(message.at(i));
    }
    socket->write(reversedMessage.toUtf8());
}
