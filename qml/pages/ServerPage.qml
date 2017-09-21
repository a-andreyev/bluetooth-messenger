import QtQuick 2.0
import Sailfish.Silica 1.0
import bluetooth.messenger 1.0

Page {
    allowedOrientations: Orientation.All
    MessengerServer {
        id: messengerServer
        onMessageReceived: {
            console.log("messageReceived()");
            messageLabel.text = message;
        }
    }
    Column {
        width: parent.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Bluetooth messenger server"
        }
        Label {
            id: messageLabel
            width: parent.width
            text: "Message text"
        }
        Button {
            id: startButton
            width: parent.width
            text: "Start server"
            onClicked: {
                startButton.enabled = false;
                messengerServer.startServer();
            }
        }
        Button {
            enabled: !startButton.enabled
            width: parent.width
            text: "Stop server"
            onClicked: {
                startButton.enabled = true;
                messengerServer.stopServer();
            }
        }
    }
}

