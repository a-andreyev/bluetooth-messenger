import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations: Orientation.All
    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: anchors.leftMargin
        spacing: Theme.paddingLarge
        PageHeader {
            title: qsTr("Bluetooth messenger")
        }
        Button {
            width: parent.width
            text: qsTr("Server mode")
            onClicked: pageStack.push(Qt.resolvedUrl("ServerPage.qml"))
        }
        Button {
            width: parent.width
            text: qsTr("Client mode")
            onClicked: pageStack.push(Qt.resolvedUrl("ClientPage.qml"))
        }
        TextArea {
            readOnly: true
            width: parent.width
            label: qsTr("Hint")
            text: qsTr("Do not forget to pair the devices
in system settings before playing with application")
        }

        Button {
            width: parent.width
            text: qsTr("Open Bluetooth Settings")
            onClicked: bluetoothMessenger.startBT()
        }
    }
}

