import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtCore

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
                    //


                    focus: true

                    onClicked: {
                        container.color = text.toLowerCase();
                        console.log("Button clicked:", text);

                        // Update the current index to the clicked button's index
                        buttonGrid.currentIndex = index;

                        // Update the focus indicator's position
                        focusIndicator.x = buttonGrid.x + x
                        focusIndicator.y = buttonGrid.y + y
                        focusIndicator.width = width
                        focusIndicator.height = height
                        focusIndicator.visible = true
                        forceActiveFocus(); // Ensure the clicked button gets focused
                    }

                    onActiveFocusChanged: {
                        if (activeFocus) {
                            focusIndicator.x = buttonGrid.x + x
                            focusIndicator.y = buttonGrid.y + y
                            focusIndicator.width = width
                            focusIndicator.height = height
                            focusIndicator.visible = true
                            console.log("Button focused:", text);
                        } else {
                            focusIndicator.visible = false
                        }
                    }

                    Keys.onTabPressed: {
                        console.log("Tab key pressed");
                        buttonGrid.currentIndex = (buttonGrid.currentIndex + 1) % repeater.count;
                        repeater.itemAt(buttonGrid.currentIndex).forceActiveFocus();
                    }

                    Keys.onEnterPressed: {
                        console.log("Enter key pressed");
                        container.color = text.toLowerCase();
                    }

                    Keys.onLeftPressed: {
                        console.log("Left arrow key pressed");
                        moveFocus(-1, 0);
                    }

                    Keys.onRightPressed: {
                        console.log("Right arrow key pressed");
                        moveFocus(1, 0);
                    }

                    Keys.onUpPressed: {
                        console.log("Up arrow key pressed");
                        moveFocus(0, -1);
                    }

                    Keys.onDownPressed: {
                        console.log("Down arrow key pressed");
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
        // Connect to the local socket server
        console.log("Connecting to local socket...");
        LocalSocketWrapperNew.connectToServer("/tmp/opcodeClient1Socket");

        //Testing:
        //handleOpcode("MOVE_UP");

        // Connect opcode handling
        LocalSocketWrapperNew.dataReceived.connect(handleOpcode);
    }

    function handleOpcode(opcode) {
        console.log("Opcode received:", opcode);

        if (opcode === "MOVE_LEFT") {
            console.log("Handling MOVE_LEFT");
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(-1, 0);
        } else if (opcode === "MOVE_RIGHT") {
            console.log("Handling MOVE_RIGHT");
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(1, 0);
        } else if (opcode === "MOVE_UP") {
            console.log("Handling MOVE_UP");
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(0, -1);
        } else if (opcode === "MOVE_DOWN") {
            console.log("Handling MOVE_DOWN");
            repeater.itemAt(buttonGrid.currentIndex).moveFocus(0, 1);
        } else if (opcode === "ENTER") {
            console.log("Handling ENTER");
            container.color = repeater.itemAt(buttonGrid.currentIndex).text.toLowerCase();
        } else if (opcode === "TABSPACE") {
            console.log("Handling TABSPACE");
            var previousIndex = buttonGrid.currentIndex;  // Save the current index

            buttonGrid.currentIndex = (buttonGrid.currentIndex + 1) % repeater.count;
            repeater.itemAt(buttonGrid.currentIndex).forceActiveFocus();

            // Check if the focus wrapped around back to the first item
            if (buttonGrid.currentIndex === 0 && previousIndex !== 0) {
                // Focus wrapped around, so send "NOT CONSUMED"
                LocalSocketWrapperNew.sendData("NOT CONSUMED");
                console.log("TABSPACE not consumed, no more focusable items");
            } else {
                // Focus moved forward successfully
                LocalSocketWrapperNew.sendData("CONSUMED");
                console.log("TABSPACE consumed, focus moved");
            }
        }
    }
}
