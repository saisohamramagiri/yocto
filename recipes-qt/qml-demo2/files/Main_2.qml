import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick 2.0

Window {
    id:app
    width: 640
    height: 480
    visible: true
    title: qsTr("Clock")
    color: "Gray"
    Image {
        width: app.width
        height:app.height
         scale:Qt.KeepAspectRatio
        source: "cadillac.webp"     // loads cat.png
    }
    Column {
        anchors.centerIn: parent
        spacing: 50

        Text{
            id: time
            anchors.centerIn: Column
            font.pixelSize: 40
            color: "red"
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
        color: "red"
        text: "IP 000:000:000:000"
    }

}
