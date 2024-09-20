import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtCore

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Tab Navigation Example")

    // Define a list of components to navigate
    ListView {
        id: listView
        width: parent.width
        height: parent.height

        model: ListModel {
            ListElement { name: "Button 1" }
            ListElement { name: "Button 2" }
            ListElement { name: "Button 3" }
            ListElement { name: "Button 4" }
        }

        delegate: Button {
            id: button
            text: model.name
            width: parent.width
            height: 50
            focus: true
            onClicked: {
                console.log("Clicked: " + text);
                tabLocalSocketWrapper.sendAction(text); // Send action via socket
            }
        }
    }

    // Handle keyboard input
    Keys.onTabPressed: {
        if (focus === null) {
            // Set focus to the first item if no item is focused
            listView.currentIndex = 0;
        } else {
            // Move focus to the next item in the list
            listView.currentIndex = (listView.currentIndex + 1) % listView.count;
        }
        listView.currentItem.focus = true;
    }

    Keys.onEnterPressed: {
        if (listView.currentItem) {
            listView.currentItem.clicked();
        }
    }

    Component.onCompleted: {
        tabLocalSocketWrapper.connectToServer("/tmp/tabSocket"); // Adjust the path if necessary
    }
}
