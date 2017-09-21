import QtQuick 2.0
import Sailfish.Silica 1.0
import QtBluetooth 5.2

Page {
    allowedOrientations: Orientation.All

    BluetoothDiscoveryModel {
        id: discoveryModel
        discoveryMode: BluetoothDiscoveryModel.FullServiceDiscovery
        uuidFilter: "1f2d6c5b-6a86-4b30-8b4e-3990043d73f1"
        onServiceDiscovered: console.log(service.name)
        onDeviceDiscovered: console.log(device.name)
    }

    Column {
        width: parent.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Bluetooth messanger QML client"
        }
        TextField {
            id: textField
            width: parent.width
            text: "New message"
        }
        Button {
            id: sendMessageButton
            width: parent.width
            text: "Send message"
            onClicked: discoveryModel.running = true
        }
        Label {
            id: statusLabel
        }
    }
}

