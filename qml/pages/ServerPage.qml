import QtQuick 2.0
import Sailfish.Silica 1.0
import bluetooth.messenger 1.0

Page {
    allowedOrientations: Orientation.All
    MessengerServer {
        id: messengerServer
        onMessageReceived: {
            console.log(qsTr("messageReceived()"));
            messageLabel.text = message;
        }
    }
    SilicaFlickable {
        anchors.fill: parent
        PageHeader {
            id: header
            title: qsTr("Bluetooth messenger server")
        }
        Column {
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.rightMargin: anchors.leftMargin
            spacing: Theme.paddingLarge

            Button {
                id: startButton
                width: parent.width
                text: qsTr("Start server")
                onClicked: {
                    startButton.enabled = false;
                    messengerServer.startServer();
                }
            }
            Button {
                enabled: !startButton.enabled
                width: parent.width
                text: qsTr("Stop server")
                onClicked: {
                    startButton.enabled = true;
                    messengerServer.stopServer();
                }
            }

            TextField {
                readOnly: true
                id: messageLabel
                width: parent.width
                label: qsTr("Received message text")
            }
        }

    }


}

