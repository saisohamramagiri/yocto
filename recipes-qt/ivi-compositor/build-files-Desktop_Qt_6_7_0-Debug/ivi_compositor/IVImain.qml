// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtWayland.Compositor
import QtWayland.Compositor.IviApplication
import QtQuick.Window

WaylandCompositor {
    //! [wayland output]
    WaylandOutput {
        id:display1
        sizeFollowsWindow: true
        window: Window {
            x:Qt.application.screens[0].virtualX
            y:Qt.application.screens[0].virtualY
            width: 1024
            height: 768
            visible: true

            Rectangle {
                id: leftArea
                width: parent.width / 2
                height: parent.height
                anchors.left: parent.left
                color: "cornflowerblue"
                Text {
                    anchors.centerIn: parent
                    text: "Ivi surface with id 1111"
                }
            }
            Rectangle {
                id: rightArea
                width: parent.width / 2
                height: parent.height
                anchors.right: parent.right
                color: "burlywood"
                Text {
                    anchors.centerIn: parent
                    text: "2222"
                }
            }
            Rectangle {
                id: dimming
                width: parent.width
                height: parent.height
                anchors.left: parent.left
                opacity:0.3
            }
        }
    }
    /*
    WaylandOutput {
        id:display2
        sizeFollowsWindow: true
        window: Window {
            x:Qt.application.screens[1].virtualX
            y:Qt.application.screens[1].virtualY
            width: 1024
            height: 768
            visible: true
            Rectangle {
                id: leftArea1
                width: parent.width / 2
                height: parent.height
                anchors.left: parent.left
                color: "cornflowerblue"
                Text {
                    anchors.centerIn: parent
                    text: "Ivi surface with id 3333"
                }
            }
            Rectangle {
                id: rightArea1
                width: parent.width / 2
                height: parent.height
                anchors.right: parent.right
                color: "burlywood"
                Text {
                    anchors.centerIn: parent
                    text: "Other surfaces"
                }
            }
        }
    }*/
    //! [wayland output]
    Component {
        id: chromeComponent
        ShellSurfaceItem {
            anchors.fill: parent
            onSurfaceDestroyed: destroy()
            //! [resizing]
            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            function handleResized() {
                if (width > 0 && height > 0)
                    shellSurface.sendConfigure(Qt.size(width, height));
            }
            //! [resizing]
        }
    }
    //! [connecting]
    IviApplication {
        onIviSurfaceCreated: (iviSurface) =>  {
                                 var surfaceArea ;
                                 if(iviSurface.iviId === 1111)
                                 {
                                     surfaceArea = leftArea;
                                 }
                                 else if(iviSurface.iviId === 2222)
                                 {
                                     surfaceArea = rightArea;
                                 }
                                 else if(iviSurface.iviId === 3333)
                                 {
                                     surfaceArea = leftArea1;
                                 }
                                 else
                                 {
                                     surfaceArea = dimming;
                                 }

            //var surfaceArea = iviSurface.iviId === 1337 ? leftArea : rightArea;
            var item = chromeComponent.createObject(surfaceArea, { "shellSurface": iviSurface } );
            //item.handleResized();
             console.log("iviSurface.iviId :"+ iviSurface.iviId);
        }
    }
    //! [connecting]
}

