import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: qsTr("Arrow Key Navigation Example")

    Rectangle {
        id: container
        width: 400
        height: 300
        color: "white" // Default background color

        Grid {
            id: buttonGrid
            columns: 3
            anchors.centerIn: parent
            spacing: 10

            property int currentIndex: 0 // Track the current index manually

            Repeater {
                id: repeater
                model: ListModel {
                    ListElement { name: "Pink" }
                    ListElement { name: "Blue" }
                    ListElement { name: "Green" }
                    ListElement { name: "Black" }
                    ListElement { name: "Yellow" }
                    ListElement { name: "Purple" }
                }

                delegate: Button {
                    text: model.name
                    width: 80
                    height: 40
                    focus: true

                    onClicked: {
                        container.color = text.toLowerCase();
                    }

                    onActiveFocusChanged: {
                        if (activeFocus) {
                            focusIndicator.x = buttonGrid.x + x
                            focusIndicator.y = buttonGrid.y + y
                            focusIndicator.width = width
                            focusIndicator.height = height
                            focusIndicator.visible = true
                        } else {
                            focusIndicator.visible = false
                        }
                    }

                    Keys.onTabPressed: {
                        buttonGrid.currentIndex = (buttonGrid.currentIndex + 1) % repeater.count;
                        repeater.itemAt(buttonGrid.currentIndex).forceActiveFocus();
                    }

                    Keys.onEnterPressed: {
                        container.color = text.toLowerCase();
                    }

                    Keys.onLeftPressed: {
                        moveFocus(-1, 0);
                    }

                    Keys.onRightPressed: {
                        moveFocus(1, 0);
                    }

                    Keys.onUpPressed: {
                        moveFocus(0, -1);
                    }

                    Keys.onDownPressed: {
                        moveFocus(0, 1);
                    }

                    function moveFocus(dx, dy) {
                        var row = Math.floor(buttonGrid.currentIndex / buttonGrid.columns);
                        var col = buttonGrid.currentIndex % buttonGrid.columns;
                        col += dx;
                        row += dy;
                        if (col < 0) col = buttonGrid.columns - 1;
                        if (col >= buttonGrid.columns) col = 0;
                        if (row < 0) row = 1; // Only 2 rows
                        if (row > 1) row = 0; // Wrap around to the first row
                        var newIndex = row * buttonGrid.columns + col;
                        if (newIndex < repeater.count) {
                            buttonGrid.currentIndex = newIndex;
                            repeater.itemAt(newIndex).forceActiveFocus();
                        }
                    }
                }
            }
        }

        Rectangle {
            id: focusIndicator
            width: 80
            height: 40
            color: "transparent"
            border.color: "red"
            border.width: 2
            visible: false // Hidden by default
            z: 1 // Ensure it is on top of other items
        }
    }

    Component.onCompleted: {
        // Initially focus the first button
        buttonGrid.currentIndex = 0;
        repeater.itemAt(0).forceActiveFocus();

        // Connect opcode handling
        tabLocalSocketWrapper.opcodeReceived.connect(handleOpcode);
    }

    function handleOpcode(opcode) {
        if (opcode === "MOVE_LEFT") {
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(-1, 0);
        } else if (opcode === "MOVE_RIGHT") {
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(1, 0);
        } else if (opcode === "MOVE_UP") {
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(0, -1);
        } else if (opcode === "MOVE_DOWN") {
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(0, 1);
        } else if (opcode === "ENTER") {
            container.color = repeater.itemAt(buttonGrid.currentIndex).text.toLowerCase();
        } else if (opcode === "TABSPACE") {
            buttonGrid.currentIndex = (buttonGrid.currentIndex + 1) % repeater.count;
            repeater.itemAt(buttonGrid.currentIndex).forceActiveFocus();
        }
    }
}
