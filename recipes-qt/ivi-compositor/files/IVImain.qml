import QtQuick
import QtWayland.Compositor
import QtWayland.Compositor.IviApplication
import QtQuick.Window
import QtCore
import QtNetwork

WaylandCompositor {
    WaylandOutput {
        id: display1
        sizeFollowsWindow: true
        window: Window {
            x: Qt.application.screens[0].virtualX
            y: Qt.application.screens[0].virtualY
            width: 1024
            height: 768
            visible: true

            Rectangle {
                id: topLeftArea
                width: parent.width / 2
                height: parent.height / 2
                anchors.left: parent.left
                color: "cornflowerblue"
                border.width: 1
                Text {
                    anchors.centerIn: parent
                    text: "Ivi surface with id 1111"
                }
            }

            Rectangle {
                id: topRightArea
                width: parent.width / 2
                height: parent.height / 2
                anchors.right: parent.right
                color: "burlywood"
                border.width: 1
                Text {
                    anchors.centerIn: parent
                    text: "Ivi surface with id 2222"
                }
            }

            Rectangle {
                id: bottomLeftArea
                width: parent.width / 2
                height: parent.height / 2
                anchors.left: parent.left
                anchors.top: topLeftArea.bottom
                color: "burlywood"
                border.width: 1
                Text {
                    anchors.centerIn: parent
                    text: "Ivi surface with id 3333"
                }
            }

            Rectangle {
                id: bottomRightArea
                width: parent.width / 2
                height: parent.height / 2
                anchors.right: parent.right
                anchors.top: topRightArea.bottom
                color: "cornflowerblue"
                border.width: 1
                Text {
                    anchors.centerIn: parent
                    text: "Other Surfaces"
                }
            }

            Rectangle {
                id: dimming
                width: parent.width
                height: parent.height
                anchors.left: parent.left
                opacity: 0.1
                color: "black"
                enabled: false
                MouseArea {
                    anchors.fill: parent
                    enabled: false
                }
            }
        }
    }

    Component {
        id: chromeComponent
        ShellSurfaceItem {
            anchors.fill: parent
            onSurfaceDestroyed: destroy()
            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            function handleResized() {
                if (width > 0 && height > 0)
                    shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }

    IviApplication {
        onIviSurfaceCreated: (iviSurface) => {
            var surfaceArea;
            if (iviSurface.iviId === 1111) {
                surfaceArea = topLeftArea;
            } else if (iviSurface.iviId === 2222) {
                surfaceArea = topRightArea;
            } else if (iviSurface.iviId === 3333) {
                surfaceArea = bottomLeftArea;
            } else if (iviSurface.iviId === 9999) {
                surfaceArea = dimming;
            } else {
                surfaceArea = bottomRightArea;
            }

            var item = chromeComponent.createObject(surfaceArea, { "shellSurface": iviSurface });
            console.log("iviSurface.iviId :" + iviSurface.iviId);
        }
    }

    // property string currentSocketName: "/tmp/opcodeClient1Socket";
    // property bool isSwitching: false; // Flag to prevent multiple switches

    // Component.onCompleted: {
    //     // Start the TCP server first
    //     tcpServerHandler.startServer(51234);

    //     // Start local servers for different tasks
    //     localServerBrightness.startServer("/tmp/brightnessSocket");  // Local server for brightness control
    //     localServerOpcodeClient1.startServer("/tmp/opcodeClient1Socket");  // Local server for opcode forwarding for Client 1
    //     localServerOpcodeClient2.startServer("/tmp/opcodeClient2Socket");  // Local server for opcode forwarding for Client 2

    //     // Connect to the local server
    //     tcpServerHandler.connectToLocalServer("/tmp/opcodeClient1Socket");

    //     // Handle received data from the TCP server
    //     tcpServerHandler.dataReceived.connect(function(data) {
    //         var dataString = data.toString();
    //         if (dataString === "SWITCH_TO_CLIENT1") {
    //             currentSocketName = "/tmp/opcodeClient1Socket";
    //             isSwitching = false;
    //             console.log("Switched control to Client 1.");
    //         } else if (dataString === "SWITCH_TO_CLIENT2") {
    //             currentSocketName = "/tmp/opcodeClient2Socket";
    //             isSwitching = false;
    //             console.log("Switched control to Client 2.");
    //         } else if (dataString === "TABSPACE") {
    //             if (isSwitching) return; // Prevent multiple switches

    //             if (currentSocketName === "/tmp/opcodeClient1Socket") {
    //                 localServerOpcodeClient1.sendData("TABSPACE", currentSocketName);
    //                 localServerOpcodeClient1.opcodeDataReceived.connect(function(response) {
    //                     var trimmedResponse = response.toString().trim();
    //                     if (trimmedResponse === "NOT CONSUMED") {
    //                         console.log("Switching to Client 2");
    //                         currentSocketName = "/tmp/opcodeClient2Socket";
    //                         localServerOpcodeClient2.sendData("TABSPACE", currentSocketName);
    //                     }
    //                     isSwitching = false;
    //                 });
    //             } else if (currentSocketName === "/tmp/opcodeClient2Socket") {
    //                 localServerOpcodeClient2.sendData("TABSPACE", currentSocketName);
    //                 localServerOpcodeClient2.opcodeDataReceived.connect(function(response) {
    //                     var trimmedResponse = response.toString().trim();
    //                     if (trimmedResponse === "NOT CONSUMED") {
    //                         console.log("Switching to Client 1");
    //                         currentSocketName = "/tmp/opcodeClient1Socket";
    //                         localServerOpcodeClient1.sendData("TABSPACE", currentSocketName);
    //                     }
    //                     isSwitching = false;
    //                 });
    //             }

    //             // Set the flag to prevent multiple switches
    //             isSwitching = true;

    //         } else {
    //             // Forward opcode data to the current client for all other opcode inputs
    //             if (currentSocketName === "/tmp/opcodeClient1Socket") {
    //                 localServerOpcodeClient1.sendData(data, currentSocketName);
    //             } else if (currentSocketName === "/tmp/opcodeClient2Socket") {
    //                 localServerOpcodeClient2.sendData(data, currentSocketName);
    //             }
    //             console.log("Forwarded opcode data to local socket: " + data);
    //         }
    //     });

    //     // Handle brightness changes
    //     localServerBrightness.brightnessChanged.connect(function(value) {
    //         dimming.opacity = 1 - value;
    //         console.log("Received brightness value: " + value);
    //     });
    // }

    // // Cleanup function to disconnect signals and close sockets
    // function cleanup() {
    //     tcpServerHandler.dataReceived.disconnect();
    //     localServerBrightness.brightnessChanged.disconnect();

    //     if (localServerOpcodeClient1.isConnected()) {
    //         localServerOpcodeClient1.opcodeDataReceived.disconnect();
    //         localServerOpcodeClient1.close();
    //     }
    //     if (localServerOpcodeClient2.isConnected()) {
    //         localServerOpcodeClient2.opcodeDataReceived.disconnect();
    //         localServerOpcodeClient2.close();
    //     }

    //     console.log("Cleanup completed. All sockets closed and signals disconnected.");
    // }

    // Component.onDestruction: {
    //     cleanup();
    // }
}
