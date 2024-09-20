import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick

ApplicationWindow {
    visible: true
    id: window
    width: 640
    height: 480
    title: qsTr("Clock")
    color: "Gray"

    header: ToolBar{

        Material.background: Material.LightBlue

        ToolButton {
            id: menuButton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            //icon.source: "images/baseline-menu-24px.svg"
            onClicked: drawer.open()
        }

        Label {
            anchors.centerIn: parent
            text: "Qt6 QML Demo"
            font.pixelSize: 20
            elide: Label.ElideRight

        }

    }

    Drawer {
        id: drawer

        width: Math.min(window.width, window.height)/ 3 * 2
        height: window.height

        ListView {
            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: parent.width
                text: model.text
                highlighted: ListView.isCurrentItem
                onClicked:{
                    //drawer.close()
                    model.triggered()
                }

            }

            model: ListModel {
                ListElement {
                    text: qsTr("Open..")
                    triggered: function() { }
                }
                ListElement {
                    text: qsTr("About..")
                    triggered: function() { }
                }
            }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 50

        Text{
            id: time
            anchors.centerIn: Column
            font.pixelSize: 40
            color: "green"
            text: "00:00:00 UTC"
        }

        Button{
            id: quit
            anchors.centerIn: Column
            font.pixelSize: 25
            text: "Stop"
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
