import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations: Orientation.All
    Column {
        id: column
        width: parent.width
        spacing: Theme.paddingLarge
        PageHeader {
            title: "Bluetooth messenger"
        }
        Button {
            width: parent.width
            text: "Server"
            onClicked: pageStack.push(Qt.resolvedUrl("ServerPage.qml"))
        }
        Button {
            width: parent.width
            text: "Client"
            onClicked: pageStack.push(Qt.resolvedUrl("ClientPage.qml"))
        }
    }
}

