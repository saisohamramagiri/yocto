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
        value: 0.9 // Initial brightness value to match with compositor's initial opacity.

        onValueChanged: {
            localSocketWrapper.sendBrightnessValue(brightnessSlider.value);
            console.log("Sent brightness value: " + brightnessSlider.value);
        }
    }

    Component.onCompleted: {
        localSocketWrapper.connectToServer("/tmp/brightnessSocket");
    }

    Connections {
        target: localSocketWrapper

        function onConnected() {
            console.log("Connected to the compositor server");
            localSocketWrapper.sendBrightnessValue(brightnessSlider.value); // Send initial brightness value
        }

        function onErrorOccurred(error) {
            console.error("Failed to connect: " + error);
        }
    }
}
