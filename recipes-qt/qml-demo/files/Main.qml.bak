import QtQuick
import QtQuick.Window
import QtQuick 2.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Clock")
    color: "Gray"

    Text{
        id: time
        anchors.centerIn: parent
        font.pixelSize: 40
        color: "white"
        font.family: "Arial"
        text: "00:00"
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
        font.family: "Arial"
        text: "IP 000:000:000:000"
    }

}

