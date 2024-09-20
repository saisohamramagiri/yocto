import QtQuick

Window {
    x:300
    y:200
    width: 640
    height: 480
    visible: true
    title: qsTr("Digital Speedo App")
    flags: Qt.ToolTip | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground
    color: "transparent"

    Rectangle {
        id: mainBg
        anchors.fill: parent
        color: "transparent"//transparent
        border.color: "red"
        border.width: 3


        Text {
            id: currentSpeedId
            anchors.centerIn: parent
            text: "55"
            font.pixelSize: 220
            color: "black"
        }

    }


}
