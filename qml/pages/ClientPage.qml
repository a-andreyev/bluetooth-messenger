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
    Column {
        width: parent.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Bluetooth messenger client"
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
            onClicked: messengerClient.startDiscovery(textField.text)
        }
        Label {
            id: statusLabel
        }
    }
}

