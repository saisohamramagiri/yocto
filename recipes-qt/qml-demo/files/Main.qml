import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick 2.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Clock")
    color: "Gray"

    Column {
        anchors.centerIn: parent
        spacing: 50

        Text{
            id: time
            anchors.centerIn: Column
            font.pixelSize: 40
            color: "white"
            text: "00:00:00 UTC"
        }

        Button{
            id: quit
            anchors.centerIn: Column
            font.pixelSize: 25
            text: "Quit"
            onClicked: Qt.callLater(Qt.quit())
        }
    }

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: time.text = new Date().toLocaleTimeString()
    }

    Text{
        id: ip
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        font.pixelSize: 20
        color: "white"
        text: "IP 000:000:000:000"
    }

}
