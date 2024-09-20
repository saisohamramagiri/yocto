import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick 2.0
Item
{
Window {
            x:Qt.application.screens[0].virtualX
            y:Qt.application.screens[0].virtualY
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
Window {
            x:Qt.application.screens[1].virtualX
            y:Qt.application.screens[1].virtualY
    width: 640
    height: 480
    visible: true
    title: qsTr("Clock")
    color: "Gray"

    Column {
        anchors.centerIn: parent
        spacing: 50

        Text{
            id: time1
            anchors.centerIn: Column
            font.pixelSize: 40
            color: "white"
            text: "00:00:00 UTC"
        }

        Button{
            id: quit1
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
        id: ip1
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        font.pixelSize: 20
        color: "white"
        text: "IP 000:000:000:000"
    }

}
}
