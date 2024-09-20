import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtCore

ApplicationWindow {
    visible: true
    width: 300
    height: 200
    title: qsTr("Brightness Adjuster")

    Slider {
        id: brightnessSlider
        anchors.centerIn: parent
        width: 200
        from: 0
        to: 1
        stepSize: 0.01
        value: 0.9 // Initial brightness value (matches compositor's initial opacity)

        onValueChanged: {
            sendBrightnessValue(brightnessSlider.value);
            console.log("Sent brightness value: " + brightnessSlider.value);
        }
    }

    Component.onCompleted: {
        connectToServer();
    }

    // Local socket communication logic
    function connectToServer() {
        socket.connectToServer("/tmp/brightnessSocket");

        socket.connected.connect(function() {
            console.log("Connected to the compositor server");
            sendBrightnessValue(brightnessSlider.value); // Send initial brightness value
        });

        socket.errorOccurred.connect(function(error) {
            console.error("Failed to connect: " + error);
        });
    }

    function sendBrightnessValue(value) {
        if (socket.isOpen()) {
            socket.write(String(value).toUtf8());
        }
    }

    // QLocalSocket for communication with compositor
    QtObject {
        id: socket
        signal connected()
        signal errorOccurred(string error)

        function connectToServer(serverName) {
            var localSocket = new QLocalSocket();
            localSocket.connected.connect(connected);
            localSocket.errorOccurred.connect(errorOccurred);
            localSocket.connectToServer(serverName);
            localSocket.open();
            return localSocket;
        }

        function write(data) {
            if (localSocket && localSocket.isOpen()) {
                localSocket.write(data);
                localSocket.flush(); // Ensure the data is sent immediately
            }
        }

        function isOpen() {
            return localSocket && localSocket.isOpen();
        }

        property var localSocket: connectToServer("/tmp/brightnessSocket")
    }
}
