import QtQuick 2.0
import Sailfish.Silica 1.0
import bluetooth.messenger 1.0

Page {
    allowedOrientations: Orientation.All
    MessengerClient {
        id: messengerClient
        onMessageReceived: textField.text = message
        onClientStatusChanged: statusLabel.text = text
    }
    SilicaFlickable {
        anchors.fill: parent
        PageHeader {
            id: header
            title: qsTr("Bluetooth messenger client")
        }
        Column {
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.rightMargin: anchors.leftMargin
            spacing: Theme.paddingLarge

            TextField {
                id: textField
                width: parent.width
                text: qsTr("New message")
            }
            Button {
                id: sendMessageButton
                width: parent.width
                text: qsTr("Send message")
                onClicked: messengerClient.startDiscovery(textField.text)
            }
            Label {
                id: statusLabel
            }
        }
    }
}

